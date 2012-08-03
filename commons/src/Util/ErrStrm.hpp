#ifndef INCLUDE_UTIL_ERRSTRM_HPP_FILE
#define INCLUDE_UTIL_ERRSTRM_HPP_FILE

#include <string>
#include <sstream>
#include <utility>

namespace Util
{

struct ErrStrm
{
public:
  template<typename T>
  ErrStrm& operator<<(const T& t)
  {
    ss_ << t;
    return *this;
  }

  std::string str(void) const
  {
    return ss_.str();
  }

private:
  std::stringstream ss_;
};

}

#endif
