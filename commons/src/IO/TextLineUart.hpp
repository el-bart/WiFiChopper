#ifndef INCLUDE_IO_TEXTLINEUART_HPP_FILE
#define INCLUDE_IO_TEXTLINEUART_HPP_FILE

#include "Util/UniqueDescriptor.hpp"
#include "IO/TextLine.hpp"
#include "IO/Exception.hpp"


namespace IO
{

class TextLineUart: public TextLine
{
public:
  struct CannotOpenDevice: public Exception
  {
    explicit CannotOpenDevice(const std::string& dev);
  };

  struct ErrorIO: public Exception
  {
    ErrorIO(const std::string& dev, const char* details);
  };


  explicit TextLineUart(const std::string& dev);

private:
  virtual size_t sendSome(const Data& data, size_t skip = 0);
  virtual size_t readSome(Data& data, double timeout);

  std::string            dev_;
  Util::UniqueDescriptor fd_;
};

}

#endif
