#include <tut/tut.hpp>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>
#include <unistd.h>

#include "IO/TextLine.hpp"

using namespace std;
using namespace IO;

namespace
{

struct TestLine: public TextLine
{
  typedef std::vector<uint8_t> Queue;


  virtual size_t sendSome(const uint8_t *data, const size_t size)
  {
    ++sentCount_;
    const size_t len = min(maxSend_, size);
    sent_.reserve( sent_.size() + len );
    copy( data, data+len, back_insert_iterator<Queue>(sent_) );
    return len;
  }


  virtual size_t readSome(uint8_t *data, size_t size, const double timeout)
  {
    ++recvCount_;
    const size_t len = min( min(size, recv_.size()), maxRecv_ );
    copy( begin(recv_), begin(recv_)+len, data );
    recv_.erase( begin(recv_), begin(recv_)+len );
    return len;
  }

  Queue  sent_;
  size_t maxSend_   = 128;
  size_t sentCount_ = 0;

  Queue  recv_;
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
    TestLine::Queue q( begin(data), end(data) );
    api_.send( &q[0], q.size() );
    // test output
    string out;
    copy( begin(tl_.sent_), end(tl_.sent_), back_insert_iterator<string>(out) );
    tut::ensure_equals("invalid data sent", out, data+"\n");
  }


  void testRecv(const size_t n, const double timeout, const string expected)
  {
    unique_ptr<uint8_t[]> buf(new uint8_t[n]);
    size_t len = tl_.read(buf.get(), n, timeout);
    string out;
    copy( buf.get(), buf.get()+len, back_insert_iterator<string>(out) );
    tut::ensure_equals("invalid content read", out, expected);
  }


  void addToRecvBuffer(const string data)
  {
    copy( begin(data), end(data), back_insert_iterator<TestLine::Queue>(tl_.recv_) );
  }


  TestLine  tl_;
  TextLine &api_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/TextLine");
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
  ensure_equals("invalid number of calls to send()", tl_.sentCount_, 6+1);
}

// test exception on null
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    api_.send(nullptr, 0);
    fail("no exception thrown on nullptr");
  }
  catch(const TextLine::ParameterError&)
  { }
}

// test reading sample string
template<>
template<>
void testObj::test<4>(void)
{
  addToRecvBuffer("test string\n");
  testRecv(32, 0.0010, "test string");
}

// test exception on null
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    api_.read(nullptr, 0, 0.0010);
    fail("no exception thrown on nullptr");
  }
  catch(const TextLine::ParameterError&)
  { }
}

// test receiving in small parts
template<>
template<>
void testObj::test<6>(void)
{
  addToRecvBuffer("some test\n");
  tl_.maxRecv_=3;
  testRecv(32, 1.0, "some test");
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
    testRecv(32, 0.0, "...");
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
  testRecv(32, 0.0, "some test");
}

// test error when there is too much data to fit in provided buffer
template<>
template<>
void testObj::test<9>(void)
{
  addToRecvBuffer("some test\n");
  try
  {
    testRecv(5, 1.0, "...");
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
  addToRecvBuffer("some test\n");
  tl_.maxRecv_=2;
  try
  {
    testRecv(5, 1.0, "...");
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
    testRecv(32, 0.001, "...");
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
  testRecv(32, 0.001, "some test");
  testRecv(32, 0.001, "other data");
}

// test reading data in parts, when there is intermediate appending
template<>
template<>
void testObj::test<13>(void)
{
  addToRecvBuffer("some test\nother ");
  testRecv(32, 0.001, "some test");
  addToRecvBuffer("data\n");
  testRecv(32, 0.001, "other data");
}

// test if data does not disappear when timeout is reached
template<>
template<>
void testObj::test<14>(void)
{
  addToRecvBuffer("some test");
  try
  {
    testRecv(32, 0.0, "...");
    fail("no exception when timeout occures when no EOL is found");
  }
  catch(const TextLine::Timeout&)
  { }
  // sinish text
  addToRecvBuffer("\n");
  testRecv(32, 0.001, "some test");
}

} // namespace tut
