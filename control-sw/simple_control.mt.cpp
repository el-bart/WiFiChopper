#include <iostream>
#include <iomanip>
#include <memory>

#include "Control/Joystick.hpp"
#include "IO/ProtoRTB.hpp"
#include "IO/LineCommUart.hpp"

using namespace std;


int main(int argc, char** argv)
{
  if( argc != 1+2 )
  {
    cerr << argv[0] << " <joystick_dev> <rtb_dev>" << endl;
    return 1;
  }

  Control::InputPtr js( new Control::Joystick( argv[1], { true, {1,true}, {0,false}, true, {3,true} } ) );
  cout << "device " << js->path() << " opened" << endl;
  cout << js->name() << endl;

  IO::LineCommPtr lineComm( new IO::LineCommUart(argv[2]) );
  IO::ProtoRTB    proto( std::move(lineComm), 0.5 );

  cout << setprecision(4) << fixed;

  {
    cout << "initial setup:" << endl;
    const Control::Input::Movement mv = js->getMovement();
    cout << "\t movement: OX=" << mv.x_ << " OY=" << mv.y_ << endl;
    const double th = js->getThrottle();
    cout << "\t throttle: " << th << endl;
  }

  // main loop
  while(true)
  {
    cout << endl;
    cout << "waiting for new event... (Vin=" << proto.batteryVoltage() << "V)" << endl;
    // read
    js->update();
    const Control::Input::Movement mv = js->getMovement();
    const double                   th = js->getThrottle();
    // print
    cout << "OX=" << mv.x_ << " OY=" << mv.y_ << " Th= " << th << endl;
    // set speed to rear engine
    const IO::ProtoRTB::EngineSpeed es(0, 0, mv.x_);
    proto.engineSpeed(es);
  }

  cout << "bye..." << endl;

  return 0;
}
