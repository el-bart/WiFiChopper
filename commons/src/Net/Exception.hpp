#ifndef INCLUDE_NET_EXCEPTION_HPP_FILE
#define INCLUDE_NET_EXCEPTION_HPP_FILE

#include <string>
#include <utility>
#include <stdexcept>

namespace Net
{

struct Exception: public std::runtime_error
{
  explicit Exception(std::string details):
    std::runtime_error( "network error: " + std::move(details) )
  { }
};

}

#endif
