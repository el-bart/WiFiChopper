#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "Util/Exception.hpp"
#include "IO/LineCommUart.hpp"
#include "IO/LineCommNetwork.hpp"
#include "IO/ServerBuilder.hpp"
#include "Net/parseAddress.hpp"
#include "Crypto/readKeyFromFile.hpp"

using namespace std;


void dropPrivileges(const char* procName, const char* uidStr, const char* gidStr)
{
  assert( procName != nullptr );
  assert( uidStr   != nullptr );
  assert( gidStr   != nullptr );
  // parse input
  const uid_t uid = atoi(uidStr);
  const gid_t gid = atoi(gidStr);
  cout << procName << ": dropping privilages to UID=" << uid << " and GID=" << gid << endl;
  // apply new permissions
  if( setuid(uid) != 0 )
    throw Util::Exception( UTIL_LOCSTRM << "unable to set UID=" << uid << ": " << strerror(errno) );
  if( setgid(gid) != 0 )
    throw Util::Exception( UTIL_LOCSTRM << "unable to set GID=" << gid << ": " << strerror(errno) );
  // all done
  cout << procName << ": now running as UID=" << getuid() << " and GID=" << getgid() << endl;
}


void handleClient(const char* procName, IO::LineComm& remote, IO::LineComm& dev)
{
  try
  {
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
  if( argc!=1+2+1+1+2 )
  {
    cerr << argv[0] << " <host> <port> <key_file> <dev> <uid> <gid>" << endl;
    return 1;
  }

  try
  {
    cout << argv[0] << ": uChaos server is initializing..." << endl;
    IO::BuilderPtr  builder( new IO::ServerBuilder( Net::parseAddress(argv[1], argv[2]), Crypto::readKeyFromFile(argv[3]) ) );
    IO::LineCommPtr dev( new IO::LineCommUart(argv[4]) );
    dropPrivileges(argv[0], argv[5], argv[6]);

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
