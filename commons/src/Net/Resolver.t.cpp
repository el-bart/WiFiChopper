#include <tut/tut.hpp>
#include <vector>
#include <algorithm>

#include "Net/Resolver.hpp"

using namespace std;
using namespace Net;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Net/Resolver");
} // unnamed namespace


namespace tut
{

// test localhost
template<>
template<>
void testObj::test<1>(void)
{
  const Resolver r("localhost");
  ensure_equals("unexpected number of IPs", r.size(), 1);
  ensure_equals("invalid net class", int(r[0].addr_[0]), 127);
}

// test direct IP
template<>
template<>
void testObj::test<2>(void)
{
  const Resolver r("192.168.1.2");
  ensure_equals("unexpected number of IPs", r.size(), 1);
  const Resolver::value_type ip = r[0];
  ensure_equals("invalid ip/0", int(ip.addr_[0]), 192);
  ensure_equals("invalid ip/1", int(ip.addr_[1]), 168);
  ensure_equals("invalid ip/2", int(ip.addr_[2]),   1);
  ensure_equals("invalid ip/3", int(ip.addr_[3]),   2);
}

// test unknown/invalid domain
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    const Resolver r("this.domain.causes.an.error");
    fail("no exception uppon invalid domain");
  }
  catch(const Resolver::CannotResolve&)
  { }
}

// test some well-known domain
template<>
template<>
void testObj::test<4>(void)
{
  const Resolver r("google.com");
  ensure("not enough number of IPs", r.size() > 0 );
  // check if IPs are unique
  vector<IPv4> ips( r.begin(), r.end() );
  sort(ips.begin(), ips.end());
  auto it = unique(ips.begin(), ips.end());
  ensure("IPs are not uniq", it==ips.end() );
}

} // namespace tut
