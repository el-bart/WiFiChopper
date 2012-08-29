#include <iomanip>
#include <iostream>

#include "UsrInt/FrameGrabberV4L.hpp"
#include "Util/ClockTimer.hpp"

using namespace std;


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <video_device>" << endl;
    return 1;
  }

  cout << setprecision(3);

  cout << "initializing device..." << endl;
  const Util::ClockTimerRT clkInit;
  UsrInt::FrameGrabberPtr fg( new UsrInt::FrameGrabberV4L(argv[1], 800, 600) );
  const std::string winName = "image from grabber";
  cv::namedWindow( winName, CV_WINDOW_AUTOSIZE|CV_WINDOW_KEEPRATIO );
  cout << "device initialized in " << clkInit.elapsed() << " second(s)" << endl;

  // check what is the grabbing resolution
  {
    const cv::Size size = fg->size();
    cout << "grabbing resolution set to " << size.width << "x" << size.height << endl;
  }

  // some setting - tell which elements are to be used at all
  constexpr bool controlExposure     = false;
  constexpr bool controlWhiteBalance = false;

  // setting up exposure time
  if(controlExposure)
  {
    if(0)
    {
      const std::chrono::microseconds expTime = std::chrono::milliseconds(50);
      cout << "setting exposure time to " << expTime.count() << "[us]" << endl;
      fg->setExposureTime(expTime);
    }
    else
    {
      cout << "setting automatic exposure time" << endl;
      fg->setAutoExposureTime();
    }
  }

  if(controlWhiteBalance)
  {
    // enable it for now
    fg->autoWhiteBalance(true);
  }

  // main loop
  cout << "grabbing frames - press any key to stop" << endl;
  const Util::ClockTimerRT clkFps;
  const Util::ClockTimerRT clkRunning;
  bool                     whiteBalanceDisabled = false;
  size_t                   i;
  for(i=0; true; ++i)
  {
    try
    {
      // check if stop was requested
      if( cv::waitKey(10) >= 0 )
      {
        cout << endl;
        cout << "breaking on user request";
        break;
      }
      if(controlWhiteBalance)
      {
        // after 2 seconds disable automatic white balance
        if( !whiteBalanceDisabled && clkRunning.elapsed() >= 2.0 )
        {
          cout << endl << "disabling automatic white balance now" << endl;
          fg->autoWhiteBalance(false);
          whiteBalanceDisabled = true;
        }
      }
      // capture
      cv::Mat frame = fg->grab();
      cerr << ".";
      // output
      imshow( winName, frame );
    }
    catch(const std::exception& ex)
    {
      cerr << 'X' << endl;
      cerr << ex.what() << endl;
    }
  }

  // summary
  const double fpsT = clkFps.elapsed();
  cout << endl;
  cout << "processed " << i << " frames in " << fpsT << " second(s) (" << i/clkFps.elapsed() << " fps on average)" << endl;
  cout << "all done! :)" << endl;

  return 0;
}
