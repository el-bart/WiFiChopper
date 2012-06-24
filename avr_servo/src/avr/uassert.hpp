#ifndef INCLUDE_UASSERT_HPP_FILE
#define INCLUDE_UASSERT_HPP_FILE

#ifndef NDEBUG
// debug

#include "config.hpp"
#include <inttypes.h>

namespace detail
{
/** \brief helper call.
 *  \param line line number.
 *  \param file file name.
 */
void uassertInternalImplementation(uint16_t line, const char *file);
}

// NOTE: BASE_SRC_NAME (set by build process) is used here instead of __FILE__, since
//       full path takes FAR too much of the memory (over 0.5kB!) in this code
#define uassert(cond) do { if(!(cond)) detail::uassertInternalImplementation(__LINE__, BASE_SRC_NAME); } while(0)

#else
// release

#define uassert(cond) do { } while(0)

#endif

#endif
