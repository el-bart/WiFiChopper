#ifndef INCLUDE_IO_TEXTLINE_HPP_FILE
#define INCLUDE_IO_TEXTLINE_HPP_FILE

#include <chrono>
#include <cinttypes>


namespace IO
{

class TextLine
{
public:
  void send(const uint8_t *data, size_t size);
  void read(const uint8_t *data, size_t size, std::chrono::milliseconds timeout);

private:
  virtual size_t sendSome(const uint8_t *data, size_t size) = 0;
  virtual size_t readSome(const uint8_t *data, size_t size, std::chrono::milliseconds timeout) = 0;
};

}

#endif
