#include <tut/tut.hpp>

#include "nameToNumber.hpp"

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("nameToNumber");
} // unnamed namespace


namespace tut
{

// test valid convertions
template<>
template<>
void testObj::test<1>(void)
{
  int i=0;
  for(char c='a'; c<'a'+SERVO_COUNT; ++c, ++i)
    ensure_equals("invalid convertion", nameToNumber(c), i);
}

// test converting invalid name - one after
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid convertion of invalid name", nameToNumber('q'+1), -1);
}

// test converting invalid name - one before
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid convertion of invalid name", nameToNumber('a'-1), -1);
}

// test converting invalid name - random character
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid convertion of invalid name", nameToNumber('?'), -1);
}

} // namespace tut
