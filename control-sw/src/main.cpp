#include <mutex>
#include <thread>
#include <atomic>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <unistd.h>

#include "Util/timedSelect.hpp"
#include "Util/ClockTimer.hpp"
#include "Crypto/readKeyFromFile.hpp"
#include "Net/parseAddress.hpp"
#include "IO/LineCommNetwork.hpp"
#include "IO/ClientBuilder.hpp"
#include "IO/ProtoRTB.hpp"
#include "Control/Joystick.hpp"
#include "Control/MotionProcessor.hpp"
#include "UsrInt/FrameGrabberV4L.hpp"
#include "UsrInt/Display.hpp"

using namespace std;


namespace
{

void ensureInitialNeutralPosition(const char* progName, Control::Input& input)
{
  bool info = true;
  while(true)
  {
    if( Util::timedSelect( input.rawDescriptor(), 0.05 ) )
        input.update();
    else
    {
      const auto mv = input.getMovement();
      const auto th = input.getThrottle();
      if( th == 0 && mv.x_ == 0 && mv.y_ == 0 )
        break;
      if( info )
      {
        cout << progName << ": !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        cout << progName << ": your input device is NOT in the neutral position!"                 << endl;
        cout << progName << ": please fix this by pushing joystick back to the neutral position." << endl;
        cout << progName << ": !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        info = false;
      }
    }
  }

  if( !info )
  {
    cout << progName << ": ok - got the neutral position!" << endl;
    sleep(2);
  }
}


class DisplayThread
{
private:
  typedef std::unique_lock<std::mutex> Lock;

public:
  DisplayThread(UsrInt::Display display, std::atomic<bool>& quit):
    quit_( &quit ),
    display_( std::move(display) ),
    dispInfo_{ 0, {0,0,0} },
    th_( [this]
          {
            while(!*this->quit_)
            {
              try
              {
                const auto info = getInfo();
                this->display_.update(info);
                if( cv::waitKey(10) == 'q' )
                  *this->quit_ = true;
              }
              catch(const std::exception& ex)
              {
                // TODO: this should display on the screen
                cerr << "DISPLAY THREAD" << ": display error: " << ex.what() << " - proceeding..." << endl;
              }
            }
          }
       )
  { }

  ~DisplayThread(void)
  {
    *quit_=true;    // interrupt thread
    th_.join();     // wait for it to join
  }


  void update(const UsrInt::Display::Info& info)
  {
    Lock lock;
    dispInfo_ = info;
  }

  UsrInt::Display::Info getInfo(void) const
  {
    Lock lock;
    return dispInfo_;
  }

private:
  std::atomic<bool>*    quit_;
  UsrInt::Display       display_;
  UsrInt::Display::Info dispInfo_;
  mutable std::mutex    mutex_;
  std::thread           th_;
};

} // unnamed namespace



int main(int argc, char **argv)
{
  if( argc != 1+2+1+1+1 )
  {
    cerr << argv[0] << " <host> <port> <key_file> <joystick_dev> <video_dev>" << endl;
    return 1;
  }

  try
  {
    // setup printout for values
    cout << setprecision(2) << fixed;

    cout << argv[0] << ": uChaos client is initializing..." << endl;
    IO::BuilderPtr  builder( new IO::ClientBuilder( Net::parseAddress(argv[1], argv[2]), Crypto::readKeyFromFile(argv[3]) ) );

    cout << argv[0] << ": joystick initialization..." << endl;
    Control::InputPtr input( new Control::Joystick( argv[4], { true, {1,true}, {0,false}, true, {3,true} } ) );
    cout << argv[0] << ": connect to the device: " << input->name() << endl;
    ensureInitialNeutralPosition( argv[0], *input );
    const Control::MotionProcessor motion( 100/255.0, + 15/255.0 );

    cout << argv[0] << ": connecting to the server..." << endl;
    IO::LineCommPtr remote = builder->build();

    cout << argv[0] << ": initializing communication..." << endl;
    IO::ProtoRTB board( std::move(remote), 4.0 );
    cout << argv[0] << ": connected to: " << board.hello() << endl;

    cout << argv[0] << ": initializing video device..." << endl;
    UsrInt::FrameGrabberPtr fg( new UsrInt::FrameGrabberV4L(argv[5], 1200, 800) );
    cout << argv[0] << ": video device initialized to " << fg->size().width << "x" << fg->size().height << " resolution" << endl;

    cout << argv[0] << ": initializing display..." << endl;
    UsrInt::Display   display( std::move(fg), true, true );
    cout << argv[0] << ": writing flight recond movie to '" << display.outputFile() << "' file" << endl;
    std::atomic<bool> quit(false);
    DisplayThread     dispTh( std::move(display), quit );

    cout << argv[0] << ": ready to start!" << endl;

    // enable CLP - just in case...
    board.enableCLP();

    Util::ClockTimerRT rtClkCLP;
    Util::ClockTimerRT rtClkRead;
    while( !quit )
    {

      try
      {
        // check if there is something new on input
        if( Util::timedSelect( input->rawDescriptor(), 0.1 ) )
        {
          input->update();                                // read new setup
          const auto mv = input->getMovement();
          const auto th = input->getThrottle();
          const auto es = motion.transform( mv, th );     // translate this to engine speeds
          board.engineSpeed(es);                          // update new speed settings
        }

        // do other stuff once in a while
        if( rtClkCLP.elapsed() > 0.6 )
        {
          board.enableCLP();                              // CLP must be enabled all time!
          rtClkCLP.restart();
        }
        if( rtClkRead.elapsed() > 0.4 )
        {
          const auto accel   = board.accelerometer();               // read accelerometer settings
          const auto battery = board.batteryVoltage();              // read battery voltage
          const UsrInt::Display::Info info = { battery, accel };    // create info object
          dispTh.update(info);                                      // send info to thread
          rtClkRead.restart();
        }

      }
      catch(const std::exception& ex)
      {
        // TODO: this should display on the screen
        cerr << argv[0] << ": communication error: " << ex.what() << " - proceeding..." << endl;
      }

    }

    cout << argv[0] << ": exiting..." << endl;
    return 0;
  }
  catch(const std::exception& ex)
  {
    cerr << argv[0] << ": fatal error: " << ex.what() << endl;
    return 42;
  }

  // this code is never reached
  return 43;
}

