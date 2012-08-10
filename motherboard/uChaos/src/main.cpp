#include <iostream>
#include <stdexcept>

#include "IO/LineCommUart.hpp"
#include "IO/LineCommNetwork.hpp"
#include "IO/ServerBuilder.hpp"
#include "Net/parseAddress.hpp"
#include "Crypto/readKeyFromFile.hpp"

using namespace std;


void resetDevicseBuffer(const char* procName, IO::LineComm& dev)
{
  cout << procName << ": uC's buffer cleanup" << endl;
  dev.send("");         // send new line to flush uC's internal buffer
  try
  {
    while(true)
      dev.read(0.2);    // discard what is being read
  }
  catch(const IO::TextLineUart::Timeout&)
  {
    // timeout means buffer is clean and we may finish
    cout << procName << ": buffer cleanup done!" << endl;
  }
  catch(const std::exception& ex)
  {
    cerr << procName << ": unexpected error during device buffer cleaning: " << ex.what() << " - trying to proceed any way..." << endl;
  }
}


void handleClient(const char* procName, IO::LineComm& remote, IO::LineComm& dev)
{
  try
  {
    // cleanup any mess, that might be there
    resetDevicseBuffer(procName, dev);

    // client processing loop
    while(true)
    {
      cout << procName << ": awaiting command" << endl;
      string txt = remote.read(20.0);
      cout << procName << ": executing command: " << txt << endl;
      dev.send(txt);
      txt = dev.read(0.5);
      cout << procName << ": forwarding response: " << txt << endl;
      remote.send(txt);
    }
  }
  catch(const std::exception& ex)
  {
    cerr << procName << ": error: " << ex.what() << " - disconnecting from client..." << endl;
  }
}


int main(int argc, char **argv)
{
  if( argc!=1+2+1+1 )
  {
    cerr << argv[0] << " <host> <port> <key_file> <dev>" << endl;
    return 1;
  }

  try
  {
    cout << argv[0] << ": uChaos server is initializing..." << endl;
    IO::BuilderPtr  builder( new IO::ServerBuilder( Net::parseAddress(argv[1], argv[2]), Crypto::readKeyFromFile(argv[3]) ) );
    IO::LineCommPtr dev( new IO::LineCommUart(argv[4]) );

    // main client-processing loop
    while(true)
    {
      cout << argv[0] << ": waiting for new client" << endl;
      IO::LineCommPtr remote( builder->build() );
      cout << argv[0] << ": processing the client" << endl;
      handleClient( argv[0], *remote, *dev );
      cout << argv[0] << ": disconnecting from the client" << endl;
    }

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
