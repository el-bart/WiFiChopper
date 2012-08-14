#include <cassert>
#include <ctype.h>

#include "Util/hex.hpp"


namespace Util
{


std::pair<char,char> toHex(uint8_t in)
{
  constexpr char lut[] = "0123456789abcdef";
  return std::pair<char,char>( lut[in>>4], lut[in&0x0F] );
}


std::string toHex(const std::vector<uint8_t> &in)
{
  std::string out;
  for(auto it = in.begin(); it!=in.end(); ++it)
  {
    const std::pair<char,char> tmp = toHex(*it);
    out.push_back(tmp.first);
    out.push_back(tmp.second);
  }
  return out;
}


namespace
{
uint8_t parseHex(char c)
{
  if( !isxdigit(c) )
    throw std::runtime_error("invalid (i.e. non-hex) digit found");
  c = tolower(c);
  if('0'<=c && c<='9')
    return c-'0';
  assert( 'a'<=c && c<='f' );
  return 10+(c-'a');
}
} // unnamed namespace


uint8_t fromHex(const char in)
{
  return parseHex(in);
}


std::vector<uint8_t> fromHex(const std::string &in)
{
  if( (in.length()%2) != 0 )
    throw std::runtime_error("string to be parsed as hex must have len%2==0");

  std::vector<uint8_t> out;
  const size_t count = in.length()/2;
  out.reserve(count);
  for(size_t i=0; i<count; ++i)
    out.push_back( (parseHex(in[2*i])<<4) | parseHex(in[2*i+1]) );
  return out;
}

}
