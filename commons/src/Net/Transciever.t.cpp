#include <sstream>
#include <tut/tut.hpp>

#include "Net/Transciever.hpp"

using namespace std;
using namespace Net;

namespace
{
struct TestClass
{
  stringstream ss_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Net/Transciever");
} // unnamed namespace


namespace tut
{

// test IP stream operator
template<>
template<>
void testObj::test<1>(void)
{
  const Transciever::IPv4 ip={{123, 22, 6, 9}};
  ss_ << ip;
  ensure_equals("invalid stream output", ss_.str(), "123.22.6.9");
}

// test address serialization
template<>
template<>
void testObj::test<2>(void)
{
  const Transciever::Address addr={{{123, 22, 6, 9}}, 1234};
  ss_ << addr;
  ensure_equals("invalid stream output", ss_.str(), "123.22.6.9:1234");
}


// test initialization with invalid descriptor
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    Transciever t( Util::UniqueDescriptor{-1} );
    fail("no exception on invalid descriptor");
  }
  catch(const Transciever::InvalidDescriptor&)
  {
    // ok
  }
}


// 
template<>
template<>
void testObj::test<4>(void)
{
}


// 
template<>
template<>
void testObj::test<5>(void)
{
}


// 
template<>
template<>
void testObj::test<6>(void)
{
}


// 
template<>
template<>
void testObj::test<7>(void)
{
}


// 
template<>
template<>
void testObj::test<8>(void)
{
}


// 
template<>
template<>
void testObj::test<9>(void)
{
}


// 
template<>
template<>
void testObj::test<10>(void)
{
}


// 
template<>
template<>
void testObj::test<11>(void)
{
}


// 
template<>
template<>
void testObj::test<12>(void)
{
}


// 
template<>
template<>
void testObj::test<13>(void)
{
}


// 
template<>
template<>
void testObj::test<14>(void)
{
}

} // namespace tut
