#ifndef INCLUDE_UTIL_TABSIZE_HPP_FILE
#define INCLUDE_UTIL_TABSIZE_HPP_FILE

namespace Util
{

template<typename T, size_t N>
constexpr size_t tabSize(const T(&)[N])
{
  return N;
}

}

#endif
