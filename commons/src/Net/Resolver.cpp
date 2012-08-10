#include <iostream>             
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Net/Resolver.hpp"
#include "Util/tabSize.hpp"

using namespace std;            

namespace Net
{

namespace
{
bool toIP(const struct addrinfo *ai, IPv4& out)
{
  assert(ai!=nullptr);
  // skipping non-inet addresses
  if(ai->ai_family!=AF_INET)
    return false;
  // sanity check for IPv4
  if( ai->ai_addrlen != sizeof(struct sockaddr_in) )
    return false;
  // cast to proper type - ugly, but this is how it works...
  const struct sockaddr_in* ip = reinterpret_cast<const struct sockaddr_in*>(ai->ai_addr);
  if(ip==nullptr)
    return false;
  // convert
  static_assert( sizeof(out.addr_)==sizeof(ip->sin_addr.s_addr), "IPv4 address structures size does not match");
  uint8_t tmp[sizeof(IPv4::addr_)];
  memcpy( tmp, &ip->sin_addr.s_addr, sizeof(ip->sin_addr.s_addr) );
  for(size_t i=0; i<Util::tabSize(tmp); ++i)
    out.addr_[i]=tmp[i];
  // signal a success
  return true;
}


struct AddrInfoHandle
{
  explicit AddrInfoHandle(struct addrinfo* ai): ai_(ai) { }
  ~AddrInfoHandle(void) { if(ai_) freeaddrinfo(ai_); }

  // noncopyable
  AddrInfoHandle(const AddrInfoHandle&) = delete;
  AddrInfoHandle& operator=(const AddrInfoHandle&) = delete;
  // nonmovable
  AddrInfoHandle(AddrInfoHandle&&) = delete;
  AddrInfoHandle& operator=(AddrInfoHandle&&) = delete;

private:
  struct addrinfo* ai_;
};
} // unnamed namespace

Resolver::Resolver(const std::string& host)
{
  struct addrinfo* infos = nullptr;
  int ret = getaddrinfo( host.c_str(), nullptr, nullptr, &infos );
  if(ret!=0)
    throw CannotResolve( host, gai_strerror(ret) );
  const AddrInfoHandle aih(infos);  // ensure safe destruction, in case of exceptions

  // process response
  IPv4 ip{0,0,0,0};
  for(struct addrinfo* it=infos; it!=nullptr; it=it->ai_next)
  {
    // check if it possible to get IPv4 out of this entry
    if( !toIP(it, ip) )
      continue;
    // skip repeating entries
    if( find( ips_.begin(), ips_.end(), ip )!=ips_.end() )
      continue;
    // ok - it's unique
    ips_.push_back(ip);
  }

  if( ips_.empty() )
    throw CannotResolve( host, "no valid IPv4 addresses returned for host" );
}

}
