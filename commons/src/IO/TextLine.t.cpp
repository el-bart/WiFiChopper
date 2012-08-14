#include <tut/tut.hpp>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <unistd.h>

#include "IO/TextLine.hpp"

using namespace std;
using namespace IO;

namespace
{

struct TestLine: public TextLine
{
  explicit TestLine(size_t maxLineLen = 128):
    TextLine(maxLineLen)
  { }

  virtual size_t sendSome(const Data& data, const size_t skip)
  {
    ++sentCount_;
    const size_t len = min( maxSend_, data.size()-skip );
    assert( skip+len <= data.size() );
    copy( data.begin()+skip, data.begin()+skip+len, back_insert_iterator<Data>(sent_) );
    return len;
  }


  virtual size_t readSome(Data& data, const double timeout)
  {
    ++recvCount_;
    const size_t len = min( recv_.size(), maxRecv_ );
    copy( begin(recv_), begin(recv_)+len, back_insert_iterator<Data>(data) );
    recv_.erase( begin(recv_), begin(recv_)+len );
    return len;
  }

  Data   sent_;
  size_t maxSend_   = 128;
  size_t sentCount_ = 0;

  Data   recv_;
  size_t maxRecv_   = 128;
  size_t recvCount_ = 0;
};


struct TestClass
{
  TestClass(void):
    api_(tl_)
  { }


  void testSend(const string data)
  {
    tl_.sent_.clear();
    const TestLine::Data q( begin(data), end(data) );
    api_.send(q);
    // test output
    string out;
    copy( begin(tl_.sent_), end(tl_.sent_), back_insert_iterator<string>(out) );
    tut::ensure_equals("invalid data sent", out, data+"\n");
  }


  void testRecv(const double timeout, const string expected, TestLine* ptr = nullptr)
  {
    TestLine& ref = (ptr)?*ptr:tl_;
    TextLine::Data buf;
    ref.read(buf, timeout);
    string out;
    copy( buf.begin(), buf.end(), back_insert_iterator<string>(out) );
    tut::ensure_equals("invalid content read", out, expected);
  }


  void addToRecvBuffer(const string data, TestLine* ptr = nullptr)
  {
    TestLine& ref = (ptr)?*ptr:tl_;
    copy( begin(data), end(data), back_insert_iterator<TestLine::Data>(ref.recv_) );
  }


  TestLine  tl_;
  TextLine &api_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("IO/TextLine");
} // unnamed namespace


namespace tut
{

// test sending small data pack
template<>
template<>
void testObj::test<1>(void)
{
  testSend("alice has a cat");
}

// test sending data in small parts
template<>
template<>
void testObj::test<2>(void)
{
  tl_.maxSend_=3;
  testSend("this is the text");
  ensure_equals("invalid number of calls to send()", tl_.sentCount_, 6);
}

// test reading sample string
template<>
template<>
void testObj::test<4>(void)
{
  addToRecvBuffer("test string\n");
  testRecv(0.0010, "test string");
}

// test receiving in small parts
template<>
template<>
void testObj::test<6>(void)
{
  addToRecvBuffer("some test\n");
  tl_.maxRecv_=3;
  testRecv(1.0, "some test");
  ensure_equals("invalid number of calls to read()", tl_.recvCount_, 4);
}

// test timeout
template<>
template<>
void testObj::test<7>(void)
{
  addToRecvBuffer("some test\n");
  tl_.maxRecv_=3;
  try
  {
    testRecv(0.0, "...");
    fail("no error uppon timeout");
  }
  catch(const TextLine::Timeout&)
  { }
}

// test if timeout==0 means one read
template<>
template<>
void testObj::test<8>(void)
{
  addToRecvBuffer("some test\n");
  testRecv(0.0, "some test");
}

// test error when there is too much data to fit in max-sized buffer
template<>
template<>
void testObj::test<9>(void)
{
  TestLine tl(5);
  tl.maxRecv_ = 4;
  addToRecvBuffer("some test\n", &tl);
  try
  {
    testRecv(1.0, "...", &tl);
    fail("no exception when buffer is too short");
  }
  catch(const TextLine::TooMuchDataInLine&)
  { }
}

// test error when there is too much data to fit in provided buffer, in case of multiple reads
template<>
template<>
void testObj::test<10>(void)
{
  TestLine tl(5);
  addToRecvBuffer("some test\n", &tl);
  tl.maxRecv_=2;
  try
  {
    testRecv(1.0, "...", &tl);
    fail("no exception when buffer is too short");
  }
  catch(const TextLine::TooMuchDataInLine&)
  { }
}

// test exception when timeout occures, while not EOL
template<>
template<>
void testObj::test<11>(void)
{
  addToRecvBuffer("some test");
  try
  {
    testRecv(0.001, "...");
    fail("no exception when timeout occures when no EOL is found");
  }
  catch(const TextLine::Timeout&)
  { }
}

// test reading multiple lines in one step
template<>
template<>
void testObj::test<12>(void)
{
  addToRecvBuffer("some test\nother data\n");
  testRecv(0.001, "some test");
  testRecv(0.001, "other data");
}

// test reading data in parts, when there is intermediate appending
template<>
template<>
void testObj::test<13>(void)
{
  addToRecvBuffer("some test\nother ");
  testRecv(0.001, "some test");
  addToRecvBuffer("data\n");
  testRecv(0.001, "other data");
}

// test if data does not disappear when timeout is reached
template<>
template<>
void testObj::test<14>(void)
{
  addToRecvBuffer("some test");
  try
  {
    testRecv(0.0, "...");
    fail("no exception when timeout occures when no EOL is found");
  }
  catch(const TextLine::Timeout&)
  { }
  // finish text
  addToRecvBuffer("\n");
  testRecv(0.001, "some test");
}

// test sending and receiving line
template<>
template<>
void testObj::test<15>(void)
{
  tl_.send("doom");
  swap(tl_.sent_, tl_.recv_);
  ensure_equals("invalid string received", tl_.read(1.0), "doom");
}

// test sending multiple times and then receiving lines
template<>
template<>
void testObj::test<16>(void)
{
  tl_.send("doom");
  tl_.send("quake");
  swap(tl_.sent_, tl_.recv_);
  ensure_equals("invalid string received /1", tl_.read(1.0), "doom" );
  ensure_equals("invalid string received /2", tl_.read(1.0), "quake");
}

} // namespace tut
