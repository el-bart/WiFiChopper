#include <tut/tut.hpp>

#include "Util/keepInRange.hpp"

using namespace Util;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/keepInRange");
} // unnamed namespace


namespace tut
{

// test in range
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("in range failed", keepInRange(42, 10, 50), 42);
}

// test too small
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("in range failed", keepInRange(3, 10, 50), 10);
}

// test too big
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("in range failed", keepInRange(666, 10, 50), 50);
}

// test min>max
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    keepInRange(42, 60, 10);
    fail("no exception on invalid range");
  }
  catch(const Util::Exception&)
  { }
}

} // namespace tut
