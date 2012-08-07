#include <tut/tut.hpp>

#include "Util/tabSize.hpp"

using namespace Util;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/tabSize");
} // unnamed namespace


namespace tut
{

// check size of char table
template<>
template<>
void testObj::test<1>(void)
{
  const char tab[42]={};
  ensure_equals("invalid char tab size", tabSize(tab), 42);
}

// check size of double table
template<>
template<>
void testObj::test<2>(void)
{
  const double tab[16]={};
  ensure_equals("invalid double tab size", tabSize(tab), 16);
}


namespace
{
template<size_t V>
void testSize(void)
{
  static_assert(V==13, "invalid value");
}
}

// check if size is a compile-time constant
template<>
template<>
void testObj::test<3>(void)
{
  constexpr long tab[13]={};
  testSize<tabSize(tab)>();
}

} // namespace tut
