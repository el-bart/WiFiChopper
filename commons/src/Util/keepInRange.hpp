#ifndef INCLUDE_UTIL_KEEPINRANGE_HPP_FILE
#define INCLUDE_UTIL_KEEPINRANGE_HPP_FILE

#include <algorithm>

#include "Util/Exception.hpp"


namespace Util
{

template<typename T>
T keepInRange(const T& in, const T& minVal, const T& maxVal)
{
  if ( minVal > maxVal )
    throw Exception( UTIL_LOCSTRM << "minimum value is greater than maximum value" );
  return std::max( minVal, std::min( in, maxVal ) );
}

}

#endif
