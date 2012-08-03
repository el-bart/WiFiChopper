/*
 * testset1.t.cpp
 *
 * some example test set for test application.
 *
 */
#include <tut/tut.hpp>

#include "UserNamespace/Some.hpp"

using namespace UserNamespace;

namespace
{
struct TestClass
{
  bool isZero(int v) const
  {
    return v==0;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("UserNamespace/Some");
} // unnamed namespace


namespace tut
{

template<>
template<>
void testObj::test<1>(void)
{
  ensure(  isZero(0) );
  ensure( !isZero(2) );
}

} // namespace tut
