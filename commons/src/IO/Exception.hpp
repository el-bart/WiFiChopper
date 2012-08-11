#ifndef INCLUDE_IO_EXCEPTION_HPP_FILE
#define INCLUDE_IO_EXCEPTION_HPP_FILE

#include <string>
#include <algorithm>
#include <stdexcept>

#include "Util/ErrStrm.hpp"
#include "Util/LocStr.hpp"

namespace IO
{

struct Exception: public std::runtime_error
{
  explicit Exception(std::string details):
    std::runtime_error( "I/O error: " + std::move(details) )
  { }

  explicit Exception(const Util::ErrStrm& es):
    std::runtime_error( "I/O error: " + es.str() )
  { }
};

}

#endif
