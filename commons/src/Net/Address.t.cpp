#include <sstream>
#include <tut/tut.hpp>

#include "Net/Address.hpp"

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

factory tf("Net/Address");
} // unnamed namespace


namespace tut
{

// test IP stream operator
template<>
template<>
void testObj::test<1>(void)
{
  const IPv4 ip={{123, 22, 6, 9}};
  ss_ << ip;
  ensure_equals("invalid stream output", ss_.str(), "123.22.6.9");
}

// test address serialization
template<>
template<>
void testObj::test<2>(void)
{
  const Address addr={{{123, 22, 6, 9}}, 1234};
  ss_ << addr;
  ensure_equals("invalid stream output", ss_.str(), "123.22.6.9:1234");
}

} // namespace tut
