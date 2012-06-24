#include <tut/tut.hpp>

#include "ServoCtrl/ServoName.hpp"

using namespace ServoCtrl;

namespace
{
struct TestClass
{
  TestClass(void):
    s_{'f'}
  {
  }

  ServoName s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ServoCtrl/ServoName");
} // unnamed namespace


namespace tut
{

// check first servo name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid first servo", s_.first(), 'a');
}

// check last servo name
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid last servo", s_.last(), 'q');
}

// test getting name
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid name", s_.getName(), 'f');
}

// test setting via name
template<>
template<>
void testObj::test<4>(void)
{
  ServoName s('c');
  ensure_equals("invalid name", s.getName(), 'c');
}

} // namespace tut
