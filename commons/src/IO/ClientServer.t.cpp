#include <tut/tut.hpp>
#include <memory>
#include <string>
#include <iterator>
#include <algorithm>

#include "IO/ServerBuilder.hpp"
#include "IO/ClientBuilder.hpp"

using namespace std;
using namespace IO;

namespace
{
struct TestClass
{
  TestClass(void):
    addr_( {{127,0,0,1}}, 5577 ),
    // key will be random content of ram - for tests this is enough
    sb_( new ServerBuilder(addr_, key_) ),
    cb_( new ClientBuilder(addr_, key_) )
  { }

  const Net::Address         addr_;
  const LineCommNetwork::Key key_;
  unique_ptr<Builder>        sb_;
  unique_ptr<Builder>        cb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("IO/Client+Server");
} // unnamed namespace


namespace tut
{

// test building client and sever
template<>
template<>
void testObj::test<1>(void)
{
  cb_->build();
  sb_->build();
}

// test client/server I/O
template<>
template<>
void testObj::test<2>(void)
{
  LineCommPtr c = cb_->build();
  LineCommPtr s = sb_->build();
  c->send("test");
  ensure_equals("invalid response", s->read(1.0), "test");
}

} // namespace tut
