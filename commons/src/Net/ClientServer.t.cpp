#include <tut/tut.hpp>
#include <string>
#include <iterator>
#include <algorithm>

#include "Net/Server.hpp"
#include "Net/connectToServer.hpp"

using namespace std;
using namespace Net;

namespace
{
struct TestClass
{
  TestClass(void):
    addr_( {{127,0,0,1}}, 9666 )
  { }

  const Address addr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Net/Client+Server");
} // unnamed namespace


namespace tut
{

// test connection to non-opened port
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    connectToServer(addr_);
    fail("connection didn't failed");
  }
  catch(const CallError&)
  { }
}

// test opening the same port twice
template<>
template<>
void testObj::test<2>(void)
{
  const Server s1(addr_);
  try
  {
    const Server s2(addr_);
    fail("no error when opening the same address:port twice");
  }
  catch(const CallError&)
  { }
}

// test connection to the server and transmition of small data pack
template<>
template<>
void testObj::test<3>(void)
{
  // setup connection
  Server  ss(addr_);
  Channel cln=connectToServer(addr_);
  Channel srv=ss.accept();
  // write some data
  const string  msg="hello network";
  Channel::Data in( msg.begin(), msg.end() );
  cln.write(in);
  // read on the other side
  Channel::Data out;
  srv.read(out, 2.0);
  // check the result
  string msgOut;
  copy( out.begin(), out.end(), back_insert_iterator<string>(msgOut) );
  ensure_equals("invalid message", msgOut, msg);
}

} // namespace tut
