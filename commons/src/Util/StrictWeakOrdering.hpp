#ifndef INCLUDE_UTIL_SRICTWEAKORDERING_HPP_FILE
#define INCLUDE_UTIL_SRICTWEAKORDERING_HPP_FILE


namespace Util
{

template<typename CRTP>
struct StrictWeakOrdering
{
  friend bool operator >(const CRTP& lhs, const CRTP& rhs) { return rhs<lhs; }
  friend bool operator==(const CRTP& lhs, const CRTP& rhs) { return !(rhs<lhs) && !(rhs>lhs); }
  friend bool operator!=(const CRTP& lhs, const CRTP& rhs) { return !(rhs==lhs); }
  friend bool operator<=(const CRTP& lhs, const CRTP& rhs) { return lhs<rhs || lhs==rhs; }
  friend bool operator>=(const CRTP& lhs, const CRTP& rhs) { return lhs>rhs || lhs==rhs; }
};

}

#endif
