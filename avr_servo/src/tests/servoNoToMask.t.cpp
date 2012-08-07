#include <tut/tut.hpp>

#include "servoNoToMask.hpp"

namespace
{
struct TestClass
{
  TestClass(void)
  {
    e_.time_  =42;
    e_.maskPB_=0xFF;
    e_.maskPC_=0xFF;
    e_.maskPD_=0xFF;
  }

  void check(const uint8_t pb, const uint8_t pc, const uint8_t pd) const
  {
    tut::ensure_equals("invalid value of port B", (int)e_.maskPB_, (int)(0xFF&(~pb)) );
    tut::ensure_equals("invalid value of port C", (int)e_.maskPC_, (int)(0xFF&(~pc)) );
    tut::ensure_equals("invalid value of port D", (int)e_.maskPD_, (int)(0xFF&(~pd)) );
  }

  Entry e_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("servoNoToMask");
} // unnamed namespace


namespace tut
{

//
// PORT B
//

// test servo 0
template<>
template<>
void testObj::test<1>(void)
{
  servoNoToMask(0, e_);
  check(1<<0, 0<<0, 0<<0);
}

// test servo 1
template<>
template<>
void testObj::test<2>(void)
{
  servoNoToMask(1, e_);
  check(1<<1, 0<<0, 0<<0);
}

// test servo 2
template<>
template<>
void testObj::test<3>(void)
{
  servoNoToMask(2, e_);
  check(1<<2, 0<<0, 0<<0);
}

// test servo 3
template<>
template<>
void testObj::test<4>(void)
{
  servoNoToMask(3, e_);
  check(1<<3, 0<<0, 0<<0);
}

// test servo 4
template<>
template<>
void testObj::test<5>(void)
{
  servoNoToMask(4, e_);
  check(1<<4, 0<<0, 0<<0);
}

// test servo 5
template<>
template<>
void testObj::test<6>(void)
{
  servoNoToMask(5, e_);
  check(1<<5, 0<<0, 0<<0);
}

//
// PORT C
//

// test servo 6
template<>
template<>
void testObj::test<7>(void)
{
  servoNoToMask(6, e_);
  check(0<<0, 1<<0, 0<<0);
}

// test servo 7
template<>
template<>
void testObj::test<8>(void)
{
  servoNoToMask(7, e_);
  check(0<<0, 1<<1, 0<<0);
}

// test servo 8
template<>
template<>
void testObj::test<9>(void)
{
  servoNoToMask(8, e_);
  check(0<<0, 1<<2, 0<<0);
}

// test servo 9
template<>
template<>
void testObj::test<10>(void)
{
  servoNoToMask(9, e_);
  check(0<<0, 1<<3, 0<<0);
}

// test servo 10
template<>
template<>
void testObj::test<11>(void)
{
  servoNoToMask(10, e_);
  check(0<<0, 1<<4, 0<<0);
}

// test servo 11
template<>
template<>
void testObj::test<12>(void)
{
  servoNoToMask(11, e_);
  check(0<<0, 1<<5, 0<<0);
}

//
// PORT C
//

// test servo 12
template<>
template<>
void testObj::test<13>(void)
{
  servoNoToMask(12, e_);
  check(0<<0, 0<<0, 1<<3);
}

// test servo 13
template<>
template<>
void testObj::test<14>(void)
{
  servoNoToMask(13, e_);
  check(0<<0, 0<<0, 1<<4);
}

// test servo 14
template<>
template<>
void testObj::test<15>(void)
{
  servoNoToMask(14, e_);
  check(0<<0, 0<<0, 1<<5);
}

// test servo 15
template<>
template<>
void testObj::test<16>(void)
{
  servoNoToMask(15, e_);
  check(0<<0, 0<<0, 1<<6);
}

// test servo 16
template<>
template<>
void testObj::test<17>(void)
{
  servoNoToMask(16, e_);
  check(0<<0, 0<<0, 1<<7);
}

//
// misc
//

// test all servos
template<>
template<>
void testObj::test<18>(void)
{
  for(int i=0; i<SERVO_COUNT; ++i)
    servoNoToMask(i, e_);
  check(0b00111111, 0b00111111, 0b11111000);
}

} // namespace tut
