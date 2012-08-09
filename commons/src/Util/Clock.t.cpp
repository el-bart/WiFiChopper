#include <tut/tut.hpp>
#include <unistd.h>

#include "Util/Clock.hpp"

using namespace std::chrono;
using namespace Util;

namespace
{
struct TestClass
{
  const Clock clk_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/Clock");
} // unnamed namespace


namespace tut
{

// test copyability
template<>
template<>
void testObj::test<1>(void)
{
  const Clock tmp1=clk_;
  Clock tmp2;
  tmp2=tmp1;
}

// test time pass
template<>
template<>
void testObj::test<2>(void)
{
  const auto d1 = clk_.elapsed<microseconds>();
  usleep(1);
  const auto d2 = clk_.elapsed<microseconds>();
  ensure("values do not differ enough", d1<d2);
}

// test conversion to milliseconds
template<>
template<>
void testObj::test<3>(void)
{
  usleep(1*1000);
  const auto d = clk_.elapsed<milliseconds>();
  ensure("too short time elapsed", d >= milliseconds(1) );
}

} // namespace tut
