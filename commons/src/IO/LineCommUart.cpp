#include "IO/LineCommUart.hpp"

namespace IO
{

LineCommUart::LineCommUart(const std::string& dev):
  tlu_(dev)
{ }


void LineCommUart::send(const std::string& line)
{
  tlu_.send(line);
}


std::string LineCommUart::read(const double timeout)
{
  return tlu_.read(timeout);
}

}
