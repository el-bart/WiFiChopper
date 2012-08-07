#ifndef INCLUDE_TABLE_HPP_FILE
#define INCLUDE_TABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "uassert.hpp"

/** \brief table of given elements
 */
template<typename T, uint8_t N>
class Table
{
public:
  /** \brief value held inside. */
  typedef T value_type;

  /** \brief access operator - const version.
   *  \param n table entry to use.
   *  \return servo's entry.
   */
  T operator[](const uint8_t n) const
  {
    uassert(n<N);
    return t_[n];
  }

  /** \brief access operator - non-const version.
   *  \param n table entry to use.
   *  \return servo's entry.
   */
  T &operator[](const uint8_t n)
  {
    uassert(n<N);
    return t_[n];
  }

private:
  T t_[N];
}; // class Table

#endif
