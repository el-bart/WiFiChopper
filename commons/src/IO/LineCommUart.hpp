#ifndef INCLUDE_IO_LINECOMMUART_HPP_FILE
#define INCLUDE_IO_LINECOMMUART_HPP_FILE

#include "IO/LineComm.hpp"
#include "IO/TextLineUart.hpp"

namespace IO
{

class LineCommUart: public LineComm
{
public:
  explicit LineCommUart(const std::string& dev);

  virtual void send(const std::string& line);
  virtual std::string read(const double timeout);

private:
  TextLineUart tlu_;
};

}

#endif
