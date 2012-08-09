#ifndef INCLUDE_IO_TEXTLINE_HPP_FILE
#define INCLUDE_IO_TEXTLINE_HPP_FILE

#include <vector>
#include <algorithm>
#include <inttypes.h>

#include "IO/Exception.hpp"


namespace IO
{

class TextLine
{
public:
  struct Timeout: public Exception
  {
    explicit Timeout(void):
      Exception("timed out while waiting for data")
    { }
  };

  struct TooMuchDataInLine: public Exception
  {
    explicit TooMuchDataInLine(void):
      Exception("too much data in a single line")
    { }
  };

  struct SendError: public Exception
  {
    explicit SendError(std::string details):
      Exception("unable to send data: " + std::move(details))
    { }
  };

  struct ParameterError: public Exception
  {
    explicit ParameterError(std::string details):
      Exception("invalid parameter: " + std::move(details))
    { }
  };


  virtual ~TextLine(void) { }

  template<size_t N>
  void send(const uint8_t (&data)[N])
  { send(data, N); }
  void send(const uint8_t *data, size_t size);

  template<size_t N>
  size_t read(uint8_t (&data)[N], const double timeout)
  { return read(data, N, timeout); }
  size_t read(uint8_t *data, size_t size, double timeout);

private:
  template<size_t N>
  size_t sendSome(const uint8_t (&data)[N])
  { return sendSome(data, N); }
  virtual size_t sendSome(const uint8_t *data, size_t size) = 0;

  template<size_t N>
  size_t readSome(uint8_t (&data)[N], const double timeout)
  { return readSome(data, N, timeout); }
  virtual size_t readSome(uint8_t *data, size_t size, double timeout) = 0;

  typedef std::vector<uint8_t> Buffer;

  Buffer buf_;
};

}

#endif
