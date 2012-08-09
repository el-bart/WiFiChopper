#include <tut/tut.hpp>
#include <unistd.h>

#include "Util/TimeoutClock.hpp"

using namespace std::chrono;
using namespace Util;

namespace
{
struct TestClass
{
  typedef TimeoutClock<microseconds> ToutClk;
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
  const ToutClk tc( microseconds(5*1000*1000) );
  ensure("already timed out?!", tc.remaining() > microseconds(0) );
}

// test timeout
template<>
template<>
void testObj::test<2>(void)
{
  const ToutClk tc( microseconds(1) );
  usleep(1);
  ensure("not timed out", tc.remaining() == microseconds(0) );
}

// test long-timed-out
template<>
template<>
void testObj::test<3>(void)
{
  const ToutClk tc( microseconds(1) );
  usleep(5);
  ensure("not timed out", tc.remaining() == microseconds(0) );
}

} // namespace tut
