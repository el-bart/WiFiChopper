#include <algorithm>
#include <cstring>
#include <cerrno>

#include "Net/Exception.hpp"
#include "Util/ErrStrm.hpp"


namespace Net
{

Exception::Exception(std::string details):
  std::runtime_error( "network error: " + std::move(details) )
{ }

Exception::Exception(const Address &addr, std::string details):
  std::runtime_error( (Util::ErrStrm{}<<"network error while talking with "<<addr<<" - "<<std::move(details)).str().c_str() )
{ }


CallError::CallError(const Address &addr, const char *call):
  Exception(addr, (Util::ErrStrm{}<<"error calling "<<call<<"(): "<<strerror(errno)).str().c_str() )
{ }

Timeout::Timeout(const Address &addr):
  Exception(addr, "timeout occured while waiting for data")
{ }

}
