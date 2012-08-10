#include <cstring>

#include "Net/Address.hpp"


namespace Net
{

IPv4::IPv4(in_addr_t in)
{
  static_assert( sizeof(in)==sizeof(addr_), "addresses size missmatch" );
  memcpy( addr_, &in, sizeof(addr_) );
}


in_addr_t IPv4::toInAddr(void) const
{
  in_addr_t tmp;
  static_assert( sizeof(tmp)==sizeof(addr_), "addresses size missmatch" );
  memcpy( &tmp, addr_, sizeof(tmp) );
  return tmp;
}

}
