#include <iostream>
#include <stdexcept>

#include "IO/LineCommNetwork.hpp"
#include "IO/ClientBuilder.hpp"
#include "Net/parseAddress.hpp"
#include "Crypto/readKeyFromFile.hpp"

using namespace std;


int main(int argc, char **argv)
{
  if( argc!=1+2+1 )
  {
    cerr << argv[0] << " <host> <port> <key_file>" << endl;
    return 1;
  }

  try
  {
    cout << argv[0] << ": uChaos client is initializing..." << endl;
    IO::BuilderPtr  builder( new IO::ClientBuilder( Net::parseAddress(argv[1], argv[2]), Crypto::readKeyFromFile(argv[3]) ) );
    cout << argv[0] << ": connecting to the server..." << endl;
    IO::LineCommPtr remote = builder->build();

    // TODO: actions here:
    remote->send("hello");
    cout << "> " << remote->read(2.0) << endl;
    remote->send("accel?");
    cout << "> " << remote->read(2.0) << endl;
    remote->send("vin?");
    cout << "> " << remote->read(2.0) << endl;
    remote->send("eng?");
    cout << "> " << remote->read(2.0) << endl;

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

