#ifndef INCLUDE_UTILS_HPP_FILE
#define INCLUDE_UTILS_HPP_FILE

#include "config.hpp"

/** \brief constant expression returning array size.
 */
template<typename T, uint8_t N>
constexpr uint8_t tabSize(const T(&)[N]) { return N; }

#endif
