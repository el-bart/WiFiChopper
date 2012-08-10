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
  explicit Exception(std::string details);
  Exception(const Address &addr, std::string details);
};


struct CallError: public Exception
{
  CallError(const Address &addr, const char *call);
};

struct Timeout: public Exception
{
  Timeout(const Address &addr);
};

}

#endif
