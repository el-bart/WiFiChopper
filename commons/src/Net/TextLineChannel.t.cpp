#include <tut/tut.hpp>
#include <string>
#include <iterator>
#include <algorithm>

#include "Net/Server.hpp"
#include "Net/connectToServer.hpp"
#include "Net/TextLineChannel.hpp"

using namespace std;
using namespace Net;

namespace
{
struct TestClass
{
  TestClass(void):
    addr_( {{127,0,0,1}}, 9666 ),
    srvSock_(addr_),
    txtCln_( connectToServer(addr_) ),
    txtSrv_( srvSock_.accept() )
  { }

  const Address   addr_;
  Server          srvSock_;
  TextLineChannel txtCln_;
  TextLineChannel txtSrv_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Net/TextLineChannel");
} // unnamed namespace


namespace tut
{

// test sending something short
template<>
template<>
void testObj::test<1>(void)
{
  txtSrv_.send("ala ma kota");
  ensure_equals("invalid string read", txtCln_.read(1.0), "ala ma kota");
}

} // namespace tut
