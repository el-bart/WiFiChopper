#ifndef INCLUDE_UTIL_LOCSTR_HPP_FILE
#define INCLUDE_UTIL_LOCSTR_HPP_FILE

#include "Util/ErrStrm.hpp"

#define UTIL_LOCSTRM (Util::ErrStrm{}<<__FILE__<<":"<<__LINE__<<" "<<__FUNCTION__<<"(): ")
#define UTIL_LOCSTR  (UTIL_LOCSTRM.str())

#endif
