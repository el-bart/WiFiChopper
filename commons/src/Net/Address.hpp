#ifndef INCLUDE_NET_ADDRESS_HPP_FILE
#define INCLUDE_NET_ADDRESS_HPP_FILE

#include <cinttypes>

#include "Util/tabSize.hpp"


namespace Net
{

struct IPv4
{
  uint8_t addr_[4];
};

struct Address
{
  IPv4     ip_;
  uint16_t port_;
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

template<typename S>
S&& operator<<(S&& strm, const Address& addr)
{
  strm << addr.ip_ << ':' << static_cast<int>(addr.port_);
  return strm;
}


inline bool operator<(const IPv4& lhs, const IPv4& rhs)
{
  for(size_t i=0; i<Util::tabSize(lhs.addr_); ++i)
  {
    if( lhs.addr_[i] < rhs.addr_[i] )
      return true;
    if( lhs.addr_[i] == rhs.addr_[i] )
      continue;
    return false;
  }
  // all elements are equal
  return false;
}


inline bool operator<(const Address& lhs, const Address& rhs)
{
  // ip is enough?
  if( lhs.ip_<rhs.ip_ )
    return true;
  // IP is not equal?
  if( rhs.ip_<lhs.ip_ )
    return false;
  // if IP is equal, compare by ports.
  return lhs.port_<rhs.port_;
}

}

#endif
