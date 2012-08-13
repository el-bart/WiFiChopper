#include <tut/tut.hpp>
#include <algorithm>

#include "Net/TextLineChannel.hpp"
#include "Net/ChannelPairHelper.t.hpp"

using namespace std;
using namespace Net;

namespace
{
struct TestClass: public ChannelPairHelper
{
  TestClass(void):
    txtCln_( std::move(c1_) ),
    txtSrv_( std::move(c2_) )
  { }

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
