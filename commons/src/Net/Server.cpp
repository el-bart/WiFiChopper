#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Net/Server.hpp"
#include "Util/ErrStrm.hpp"


namespace Net
{

Server::Server(Address addr):
  addr_( std::move(addr) ),
  srv_( openSocket() )
{ }


Channel Server::accept(void)
{
  // wait for connection from client
  struct sockaddr_in cAddr;
  socklen_t          cAddrLen = sizeof(cAddr);
  Util::UniqueDescriptor cln( ::accept( srv_.get(), reinterpret_cast<struct sockaddr*>(&cAddr), &cAddrLen ) );
  if( cln.get()==-1 )
    throw CallError(addr_, "accept");
  // save her address
  const Address addr = { IPv4(cAddr.sin_addr.s_addr), ntohs(cAddr.sin_port) };
  // return final object
  return Channel( addr, std::move(cln) );
}


Util::UniqueDescriptor Server::openSocket(void) const
{
  // open socket
  Util::UniqueDescriptor sock( socket(AF_INET, SOCK_STREAM, 0) );
  if( sock.get()==-1 )
    throw CallError(addr_, "socket");

  // prepare address info
  struct sockaddr_in sAddr;
  memset( &sAddr, 0,  sizeof(sAddr) );
  sAddr.sin_family      = AF_INET;
  sAddr.sin_port        = htons(addr_.port_);
  sAddr.sin_addr.s_addr = addr_.ip_.toInAddr();

  // prevent isses with socket being already busy
  {
    const int value = 1;
    if( setsockopt( sock.get(), SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value) ) == -1 )
      throw CallError(addr_, "setsockopt");
  }

  // turns on keep alive messages
  {
    const int value = 1;
    if( setsockopt( sock.get(), SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value) ) == -1 )
      throw CallError(addr_, "setsockopt");
  }

  // bind to socket to address
  if( bind( sock.get(), reinterpret_cast<struct sockaddr*>(&sAddr), sizeof(sAddr) ) == -1 )
    throw CallError(addr_, "bind");

  // listen for incomming connections
  if( listen( sock.get(), 1 ) == -1 )
    throw CallError(addr_, "listen");

  // return binded and configured socket
  return sock;
}

}
