#include <tut/tut.hpp>

#include "ChronoTable.hpp"

namespace
{
struct TestClass
{
  TestClass(void):
    ct_( makeDefaultPositions() )
  {
  }

  Positions makeDefaultPositions(void) const
  {
    Positions p;
    for(int i=0; i<SERVO_COUNT; ++i)
      p[i]=200;
    return p;
  }

  void checkEnd(const EntryTable &et, const int from) const
  {
    for(int i=from; i<SERVO_COUNT; ++i)
      check(et[i], 0, 0x00, 0x00, 0x00);
  }

  void check(const Entry &e, const uint8_t etime, const uint8_t maskPB, const uint8_t maskPC, const uint8_t maskPD) const
  {
    tut::ensure_equals("invalid time stamp", (int)e.time_, (int)etime);
    tut::ensure_equals("invalid PB value", (int)e.maskPB_, (int)maskPB);
    tut::ensure_equals("invalid PC value", (int)e.maskPC_, (int)maskPC);
    tut::ensure_equals("invalid PD value", (int)e.maskPD_, (int)maskPD);
  }

  const EntryTable &et(void) const
  {
    return ct_.currentEntries();
  }

  ChronoTable ct_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ChronoTable");
} // unnamed namespace


namespace tut
{

// check if entries are fine, when set to default
template<>
template<>
void testObj::test<1>(void)
{
  check(et()[0], 200, 0b11000000, 0b11000000, 0b00000111);
  checkEnd(et(), 1);
}

// test changing some entry to a different time
template<>
template<>
void testObj::test<2>(void)
{
  ct_.currentPos()[3]=100;
  ct_.update();
  check(et()[0], 100, 0b11110111, 0b11111111, 0b11111111);
  check(et()[1], 200, 0b11001000, 0b11000000, 0b00000111);
  checkEnd(et(), 2);
}

// test entries, in different order and times
template<>
template<>
void testObj::test<3>(void)
{
  ct_.currentPos()[1]=220;
  ct_.currentPos()[8]=100;
  ct_.update();
  check(et()[0], 100, 0b11111111, 0b11111011, 0b11111111);
  check(et()[1], 200, 0b11000010, 0b11000100, 0b00000111);
  check(et()[2], 220, 0b11111101, 0b11111111, 0b11111111);
  checkEnd(et(), 3);
}

// test all servos in different positions
template<>
template<>
void testObj::test<4>(void)
{
  ct_.currentPos()[0] =10;
  ct_.currentPos()[1] =11;
  ct_.currentPos()[2] =12;
  ct_.currentPos()[3] =13;
  ct_.currentPos()[4] =14;
  ct_.currentPos()[5] =15;
  ct_.currentPos()[6] =16;
  ct_.currentPos()[7] =17;
  ct_.currentPos()[8] =18;
  ct_.currentPos()[9] =19;
  ct_.currentPos()[10]=20;
  ct_.currentPos()[11]=21;
  ct_.currentPos()[12]=22;
  ct_.currentPos()[13]=23;
  ct_.currentPos()[14]=24;
  ct_.currentPos()[15]=25;
  ct_.currentPos()[16]=26;
  ct_.update();
  check(et()[0],  10, 0b11111110, 0b11111111, 0b11111111);
  check(et()[1],  11, 0b11111101, 0b11111111, 0b11111111);
  check(et()[2],  12, 0b11111011, 0b11111111, 0b11111111);
  check(et()[3],  13, 0b11110111, 0b11111111, 0b11111111);
  check(et()[4],  14, 0b11101111, 0b11111111, 0b11111111);
  check(et()[5],  15, 0b11011111, 0b11111111, 0b11111111);
  check(et()[6],  16, 0b11111111, 0b11111110, 0b11111111);
  check(et()[7],  17, 0b11111111, 0b11111101, 0b11111111);
  check(et()[8],  18, 0b11111111, 0b11111011, 0b11111111);
  check(et()[9],  19, 0b11111111, 0b11110111, 0b11111111);
  check(et()[10], 20, 0b11111111, 0b11101111, 0b11111111);
  check(et()[11], 21, 0b11111111, 0b11011111, 0b11111111);
  check(et()[12], 22, 0b11111111, 0b11111111, 0b11110111);
  check(et()[13], 23, 0b11111111, 0b11111111, 0b11101111);
  check(et()[14], 24, 0b11111111, 0b11111111, 0b11011111);
  check(et()[15], 25, 0b11111111, 0b11111111, 0b10111111);
  check(et()[16], 26, 0b11111111, 0b11111111, 0b01111111);
  checkEnd(et(), 18);   // i.e. - do nothing... ;)
}

// test setting 'o' servo to 0x00 (bug-test)
template<>
template<>
void testObj::test<5>(void)
{
  ct_.currentPos()['o'-'a']=0x00;
  ct_.update();
  check(et()[0],   0, 0b11111111, 0b11111111, 0b11011111);
  check(et()[1], 200, 0b11000000, 0b11000000, 0b00100111);
  checkEnd(et(), 2);
}

} // namespace tut
