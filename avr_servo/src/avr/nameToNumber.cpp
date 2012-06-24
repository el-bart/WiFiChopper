#include "config.hpp"
#include "nameToNumber.hpp"

int nameToNumber(char n)
{
  if(n<'a' || 'a'+SERVO_COUNT-1<n)
    return -1;
  return n-'a';
}
