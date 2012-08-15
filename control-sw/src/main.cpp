#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "Crypto/readKeyFromFile.hpp"
#include "Net/parseAddress.hpp"
#include "IO/LineCommNetwork.hpp"
#include "IO/ClientBuilder.hpp"
#include "IO/ProtoRTB.hpp"
#include "Control/Joystick.hpp"

using namespace std;


int main(int argc, char **argv)
{
  if( argc!=1+2+1+1 )
  {
    cerr << argv[0] << " <host> <port> <key_file> <joystick_dev>" << endl;
    return 1;
  }

  try
  {
    cout << argv[0] << ": uChaos client is initializing..." << endl;
    IO::BuilderPtr  builder( new IO::ClientBuilder( Net::parseAddress(argv[1], argv[2]), Crypto::readKeyFromFile(argv[3]) ) );

    cout << argv[0] << ": joystick initialization..." << endl;
    Control::InputPtr input( new Control::Joystick( argv[4], { true, {1,true}, {0,false}, true, {3,true} } ) );
    cout << argv[0] << ": connect to the device: " << input->name() << endl;

    cout << argv[0] << ": connecting to the server..." << endl;
    IO::LineCommPtr remote = builder->build();

    cout << argv[0] << ": initializing communication..." << endl;
    IO::ProtoRTB board( std::move(remote), 4.0 );
    cout << argv[0] << ": connected to: " << board.hello() << endl;

    // TODO

    cout << argv[0] << "exiting..." << endl;
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

