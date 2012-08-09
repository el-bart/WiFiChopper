#include <iomanip>
#include <tut/tut.hpp>
#include <unistd.h>

#include "Util/TimeoutClock.hpp"

using namespace Util;

namespace
{
struct TestClass
{
  typedef TimeoutClock ToutClk;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/TimeoutClock");
} // unnamed namespace


namespace tut
{

// test long-before-timeout
template<>
template<>
void testObj::test<1>(void)
{
  const ToutClk tc(5.0);
  ensure("already timed out?!", tc.remaining() > 0 );
}

// test timeout
template<>
template<>
void testObj::test<2>(void)
{
  const ToutClk tc(0.000001);
  usleep(1);
  ensure_equals("not timed out", tc.remaining(), 0 );
}

// test long-timed-out
template<>
template<>
void testObj::test<3>(void)
{
  const ToutClk tc(0.000001);
  usleep(5);
  ensure_equals("not timed out", tc.remaining(), 0 );
}

} // namespace tut
