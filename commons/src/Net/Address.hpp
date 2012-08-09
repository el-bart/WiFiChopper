#ifndef INCLUDE_NET_ADDRESS_HPP_FILE
#define INCLUDE_NET_ADDRESS_HPP_FILE

#include <cinttypes>


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

}

#endif
