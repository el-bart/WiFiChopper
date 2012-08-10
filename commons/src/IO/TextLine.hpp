#ifndef INCLUDE_IO_TEXTLINE_HPP_FILE
#define INCLUDE_IO_TEXTLINE_HPP_FILE

#include <string>
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


  typedef std::vector<uint8_t> Data;

  explicit TextLine(size_t maxLineLen=4*1024);
  virtual ~TextLine(void) { }

  // noncopyable:
  TextLine(const TextLine&) = delete;
  TextLine& operator=(const TextLine&) = delete;
  // nonmovable:
  TextLine(TextLine&&) = delete;
  TextLine& operator=(TextLine&&) = delete;

  void send(const Data& data);
  void send(const std::string& line);

  void read(Data& data, double timeout);
  std::string read(double timeout);

private:
  virtual size_t sendSome(const Data& data, size_t skip = 0) = 0;
  virtual size_t readSome(Data& data, double timeout) = 0;

  Data   eol_;
  Data   buf_;
  size_t maxLineLen_;
};

}

#endif
