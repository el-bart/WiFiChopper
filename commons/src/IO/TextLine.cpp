#include "IO/TextLine.hpp"

namespace IO
{

void TextLine::send(const uint8_t *data, const size_t size)
{
  size_t done = 0;
  size_t left = size;
  while(left>0)
  {
    const size_t bytes = sendSome(data+done, left);
    done += bytes;
    left -= bytes;
  }
}


void TextLine::read(const uint8_t *data, size_t size, std::chrono::milliseconds timeout)
{
  // TODO
}

}
