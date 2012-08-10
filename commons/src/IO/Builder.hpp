#ifndef INCLUDE_IO_BUILDER_HPP_FILE
#define INCLUDE_IO_BUILDER_HPP_FILE

#include <algorithm>

#include "IO/LineCommNetwork.hpp"


namespace IO
{

class Builder
{
public:
  typedef LineCommNetwork::Key Key;

  virtual ~Builder(void) { }

  LineCommPtr build(void) { return buildImpl(); }

private:
  virtual LineCommPtr buildImpl(void) = 0;
};

}

#endif
