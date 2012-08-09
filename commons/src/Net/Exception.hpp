#ifndef INCLUDE_NET_EXCEPTION_HPP_FILE
#define INCLUDE_NET_EXCEPTION_HPP_FILE

#include <string>
#include <stdexcept>

#include "Util/ErrStrm.hpp"
#include "Net/Address.hpp"

namespace Net
{

struct Exception: public std::runtime_error
{
  explicit Exception(std::string details):
    std::runtime_error( "network error: " + details )
  { }

  Exception(const Address &addr, std::string details):
    std::runtime_error( (Util::ErrStrm{}<<"network error while talking with "<<addr<<" - "<<details).str().c_str() )
  { }
};

}

#endif
