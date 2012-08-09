#include <tut/tut.hpp>
#include <algorithm>
#include <cstring>

#include "Crypto/BinData.hpp"

using namespace Crypto;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/BinData");
} // unnamed namespace


namespace tut
{

// check allocation
template<>
template<>
void testObj::test<1>(void)
{
  const BinData bd(42);
  ensure_equals("invalid size", bd.size(), 42);
}

// test moveability
template<>
template<>
void testObj::test<2>(void)
{
  BinData bd1(42);
  BinData bd2( std::move(bd1) );
  ensure_equals("invalid size", bd2.size(), 42);
}

// test modifications
template<>
template<>
void testObj::test<3>(void)
{
  BinData bd(3);
  memcpy(bd.data(), "abc", 3);
  ensure_equals("invalid byte 1", bd.data()[0], 'a');
  ensure_equals("invalid byte 2", bd.data()[1], 'b');
  ensure_equals("invalid byte 3", bd.data()[2], 'c');
}

} // namespace tut
