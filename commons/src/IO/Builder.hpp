#ifndef INCLUDE_IO_BUILDER_HPP_FILE
#define INCLUDE_IO_BUILDER_HPP_FILE

#include <algorithm>

#include "IO/LineComm.hpp"


namespace IO
{

class Builder
{
public:
  typedef LineComm::Key Key;

  virtual ~Builder(void) { }

  LineComm build(void) { return buildImpl(); }

private:
  virtual LineComm buildImpl(void) = 0;
};

}

#endif
