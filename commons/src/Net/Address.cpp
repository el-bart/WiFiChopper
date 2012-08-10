#include <cstring>

#include "Net/Address.hpp"


namespace Net
{

in_addr_t IPv4::toInAddr(void) const
{
  in_addr_t tmp;
  static_assert( sizeof(tmp)==sizeof(addr_), "addresses size missmatch" );
  memcpy( &tmp, addr_, sizeof(addr_) );
  return tmp;
}

}
