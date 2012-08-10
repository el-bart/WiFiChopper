#include <tut/tut.hpp>
#include <algorithm>

#include "Net/Channel.hpp"
#include "Net/SocketPairHelper.t.hpp"

using namespace std;
using namespace Net;

namespace
{

struct TestClass: public SocketPairHelper
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Net/Channel");
} // unnamed namespace


namespace tut
{

// test writing to one end and reading from the other
template<>
template<>
void testObj::test<1>(void)
{
  {
    const Channel::Data d={'a', 'b'};
    c1_.write(d);
  }
  {
    Channel::Data d;
    c2_.read(d, 1.0);
    ensure_equals("invalid number of bytes read", d.size(), 2);
    ensure_equals("invalid byte 0", d[0], 'a');
    ensure_equals("invalid byte 1", d[1], 'b');
  }
}

// test read/write more data
template<>
template<>
void testObj::test<2>(void)
{
  Channel::Data data;
  for(int i=0; i<4*1024; ++i)
    data.push_back(i%0xFF);
  c1_.write(data);

  Channel::Data rd;
  c2_.read(rd, 1.0);
  ensure_equals("invalid number of bytes", rd.size(), data.size() );
}

// test timeout
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    Channel::Data data;
    c1_.read(data, 0.0);
    fail("no exception on timeout");
  }
  catch(const std::runtime_error&)
  { }
}

// test movability
template<>
template<>
void testObj::test<4>(void)
{
  c1_ = std::move(c2_);
  Channel tmp( std::move(c1_) );
}

} // namespace tut
