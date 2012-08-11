#ifndef INCLUDE_NONCOPYABLE_HPP_FILE
#define INCLUDE_NONCOPYABLE_HPP_FILE

#include "config.hpp"

/** \brief boost::noncopyable clone.
 */
class Noncopyable
{
public:
  /** \brief use as a base class is prefered, but not forced.
   */
  Noncopyable(void)
  {
  }

private:
  // block copyablility
  Noncopyable(const Noncopyable&);
  const Noncopyable &operator=(const Noncopyable&);
}; // class Noncopyable

#endif
