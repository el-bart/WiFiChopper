#include <algorithm>
#include <cstring>

#include "Net/connectToServer.hpp"


namespace Net
{

Channel connectToServer(const Address& addr)
{
  // open socket
  Util::UniqueDescriptor sock( socket(AF_INET, SOCK_STREAM, 0) );
  if( sock.get()==-1 )
    throw CallError(addr, "socket");

  // prepare address info
  struct sockaddr_in sAddr;
  memset( &sAddr, 0,  sizeof(sAddr) );
  sAddr.sin_family      = AF_INET;
  sAddr.sin_port        = htons(addr.port_);
  sAddr.sin_addr.s_addr = addr.ip_.toInAddr();

  // connect to remove host via socket
  if( connect( sock.get(), reinterpret_cast<const sockaddr*>(&sAddr), sizeof(sAddr) ) == -1 )
    throw CallError(addr, "connect");

  // return connected socket
  return Channel( addr, std::move(sock) );
}

}
