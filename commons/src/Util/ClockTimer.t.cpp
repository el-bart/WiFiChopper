#include <tut.h>
#include <cmath>
#include <unistd.h>

#include "Util/ClockTimer.hpp"

using namespace Util;

namespace
{
typedef ClockTimer<CLOCK_REALTIME> Tmr;

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("Util/ClockTimer");
}

namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Tmr t;
}

// test time measurement's incremental
template<>
template<>
void testObj::test<2>(void)
{
  const Tmr t;
  const double m1=t.elapsed();
  usleep(10*1000);
  const double m2=t.elapsed();
  ensure("10[ms] not measured", m1<m2);
}

// test reseting
template<>
template<>
void testObj::test<3>(void)
{
  Tmr t;
  usleep(80*1000);
  const double m1=t.elapsed();
  t.restart();
  usleep(10*1000);
  const double m2=t.elapsed();
  ensure("restarting timer failed", m2<m1);
}

// test if counter starts
template<>
template<>
void testObj::test<4>(void)
{
  const Tmr t;
  usleep(10*1000);
  const double m1=t.elapsed();
  ensure("timer didn't start automatically", m1>0);
}

// test copyability
template<>
template<>
void testObj::test<5>(void)
{
  Tmr t1;
  usleep(80*1000);
  Tmr t2=t1;
  ensure("too big timespan", fabs( t1.elapsed()-t2.elapsed() )<0.070 );
}

// test if result is in seconds
template<>
template<>
void testObj::test<6>(void)
{
  Tmr t;
  usleep(100*1000);
  ensure("time is not measured in [s]", fabs( t.elapsed()-0.1 )<0.05 );
}

// test if resolution is positive
template<>
template<>
void testObj::test<7>(void)
{
  Tmr t;
  while( !(t.elapsed()>0.0) ) {};
  ensure("measured time is smaller than resolution", t.elapsed() >= t.resolution() );
}

// test if resolution is positive
template<>
template<>
void testObj::test<8>(void)
{
  ensure("resolution is not positive number", Tmr::resolution()>0.0 );
}

} // namespace tut
