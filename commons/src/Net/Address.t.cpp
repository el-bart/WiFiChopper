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

// test SWO for IPv4
template<>
template<>
void testObj::test<3>(void)
{
  const IPv4 ip1={{1, 2, 3, 4}};

  const IPv4 ip2={{2, 2, 3, 4}};
  const IPv4 ip3={{1, 3, 3, 4}};
  const IPv4 ip4={{1, 2, 4, 4}};
  const IPv4 ip5={{1, 2, 3, 5}};

  // test positive case
  ensure("relation 1 fails", ip1<ip2);
  ensure("relation 2 fails", ip1<ip3);
  ensure("relation 3 fails", ip1<ip4);
  ensure("relation 4 fails", ip1<ip5);

  // test negative case
  ensure("relation 5 fails", !(ip2<ip1));
  ensure("relation 6 fails", !(ip3<ip1));
  ensure("relation 7 fails", !(ip4<ip1));
  ensure("relation 8 fails", !(ip5<ip1));

  // test equality
  ensure("relation 9 fails", !(ip1<ip1));
}

// test SWO for Address
template<>
template<>
void testObj::test<4>(void)
{
  const Address a1 = {{{1,2,3,4}}, 400};
  const Address a2 = {{{1,2,3,4}}, 500};
  const Address a3 = {{{1,2,3,4}}, 300};
  const Address a4 = {{{6,2,3,4}}, 400};
  const Address a5 = {{{0,2,3,4}}, 400};

  // test positive cases
  ensure("relateion 1 fails", a1<a2);
  ensure("relateion 2 fails", a1<a4);
  ensure("relateion 3 fails", a3<a1);
  ensure("relateion 4 fails", a5<a1);

  // negative case
  ensure("relateion 5 fails", !(a2<a1));
  ensure("relateion 6 fails", !(a4<a1));
  ensure("relateion 7 fails", !(a1<a3));
  ensure("relateion 8 fails", !(a1<a5));

  // equality
  ensure("relation 9 failes", !(a1<a1));
}

} // namespace tut
