#include <iostream>
#include <iomanip>
#include <memory>

#include "Control/Joystick.hpp"

using namespace std;

int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <joystick_dev>" << endl;
    return 1;
  }

  Control::InputPtr js( new Control::Joystick(argv[1]) );
  cout << "device " << js->path() << " opened" << endl;
  cout << js->name() << endl;

  cout << setprecision(2) << fixed;

  {
    cout << "initial setup:" << endl;
    const Control::Input::Movement mv = js->getMovement();
    cout << "\t movement: OX=" << mv.x_ << " OY=" << mv.y_ << endl;
    const double th = js->getThrottle();
    cout << "\t throttle: " << th << endl;
  }

  // main loop
  while(false)
  {
    cout << endl;
    cout << "waiting for new event..." << endl;
    js->update();
    const Control::Input::Movement mv = js->getMovement();
    const double                   th = js->getThrottle();
    cout << "OX=" << mv.x_ << " OY=" << mv.y_ << "th: " << th << endl;
  }

  cout << "bye..." << endl;

  return 0;
}
