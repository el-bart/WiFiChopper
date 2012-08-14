#include <iomanip>
#include <iostream>

#include "IO/ProtoRTB.hpp"
#include "IO/LineCommUart.hpp"

using namespace std;


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <usart_dev>" << endl;
    return 1;
  }

  IO::LineCommPtr lc( new IO::LineCommUart(argv[1]) );
  IO::ProtoRTB    proto( std::move(lc), 2.0 );

  cout << setprecision(3) << fixed;

  cout << "sending hello" << endl;
  cout << "> " << proto.hello() << endl;

  cout << "reading accelerometer" << endl;
  const IO::ProtoRTB::Accel accel = proto.accelerometer();
  cout << "> X=" << accel.x_ << " Y=" << accel.y_ << " Z=" << accel.z_ << " (|a|=" << accel.norm() << ")" << endl;

  cout << "setting engines speed" << endl;
  proto.engineSpeed( IO::ProtoRTB::EngineSpeed(0.5, 1, -0.1) );
  cout << "> <ok>" << endl;

  cout << "getting engines speed" << endl;
  const IO::ProtoRTB::EngineSpeed spd = proto.engineSpeed();
  cout << "> M1=" << spd.getMain1() << " M2=" << spd.getMain2() << " R=" << spd.getRear() << endl;

  cout << "getting battery voltage" << endl;
  cout << "> " << proto.batteryVoltage() << "V" << endl;

  return 0;
}
