#ifndef INCLUDE_NET_ADDRESS_HPP_FILE
#define INCLUDE_NET_ADDRESS_HPP_FILE

#include <algorithm>
#include <cinttypes>
#include <cassert>
#include <netinet/in.h>

#include "Util/tabSize.hpp"
#include "Util/StrictWeakOrdering.hpp"


namespace Net
{

struct IPv4: public Util::StrictWeakOrdering<IPv4>
{
  explicit IPv4(in_addr_t in);

  IPv4(std::initializer_list<uint8_t> lst)
  {
    assert( lst.size()==Util::tabSize(addr_) && "invalid number of arguments" );
    std::copy( lst.begin(), lst.end(), addr_ );
  }

  bool operator<(const IPv4& rhs) const
  {
    for(size_t i=0; i<Util::tabSize(addr_); ++i)
    {
      if( addr_[i] < rhs.addr_[i] )
        return true;
      if( addr_[i] == rhs.addr_[i] )
        continue;
      return false;
    }
    // all elements are equal
    return false;
  }

  in_addr_t toInAddr(void) const;

  uint8_t addr_[4];
};


template<typename S>
S&& operator<<(S&& strm, const IPv4& ip)
{
  bool first=true;
  for(auto f: ip.addr_)
  {
    if(first)
      first=false;
    else
      strm << '.';
    strm << static_cast<int>(f);
  }
  return strm;
}



struct Address: public Util::StrictWeakOrdering<Address>
{
  Address(const IPv4& ip, const uint16_t port):
    ip_(ip),
    port_(port)
  { }

  bool operator<(const Address& rhs) const
  {
    // ip is enough?
    if( ip_<rhs.ip_ )
      return true;
    // IP is not equal?
    if( rhs.ip_<ip_ )
      return false;
    // if IP is equal, compare by ports.
    return port_<rhs.port_;
  }

  IPv4     ip_;
  uint16_t port_;
};


template<typename S>
S&& operator<<(S&& strm, const Address& addr)
{
  strm << addr.ip_ << ':' << static_cast<int>(addr.port_);
  return strm;
}

}

#endif
