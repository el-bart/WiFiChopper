#ifndef INCLUDE_IO_EXCEPTION_HPP_FILE
#define INCLUDE_IO_EXCEPTION_HPP_FILE

#include <string>
#include <utility>
#include <stdexcept>

namespace IO
{

struct Exception: public std::runtime_error
{
  explicit Exception(std::string details):
    std::runtime_error( "I/O error: " + std::move(details) )
  { }
};

}

#endif
