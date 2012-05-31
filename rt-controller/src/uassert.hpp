/*
 * uassert.hpp
 *
 */
#ifndef INCLUDE_UASSERT_HPP_FILE
#define INCLUDE_UASSERT_HPP_FILE

#ifndef NDEBUG
// debug

#include <inttypes.h>

namespace detail
{
/** \brief helper call.
 *  \param line line number.
 *  \param file file name.
 */
void uassertInternalImplementation(uint16_t line, const char *file);
}

#define uassert(cond) do { if(!(cond)) detail::uassertInternalImplementation(__LINE__, __FILE__); } while(0)

#else
// release

#define uassert(cond) do { } while(0)

#endif

#endif
