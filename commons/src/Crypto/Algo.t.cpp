#include <tut/tut.hpp>
#include <algorithm>
#include <cstring>

#include "Crypto/Algo.hpp"
#include "Util/tabSize.hpp"

using namespace Crypto;

namespace
{

struct TestAlgo: public Algo
{
  virtual const char* name(void) const { return "somename"; }
  virtual const char* mode(void) const { return "somemode"; }
  virtual size_t blockSize(void) const { return 10; }

  virtual void encryptImpl(uint8_t* data, size_t size)
  {
    Algo::Data tmp(data, data+size);
    tmp.swap(last_);
  }

  virtual void decryptImpl(uint8_t* data, size_t size)
  {
    Algo::Data tmp(data, data+size);
    tmp.swap(last_);
  }

  Algo::Data last_;
};


struct TestClass
{
  Algo::Data data_;
  TestAlgo   ta_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/Algo");
} // unnamed namespace


namespace tut
{

// check empty input
template<>
template<>
void testObj::test<1>(void)
{
  ta_.encrypt(data_);
  // check total size
  ensure_equals("invalid number of bytes", ta_.last_.size(), 2*ta_.blockSize() );
  // ensure proper padding
  for(size_t i=ta_.blockSize(); i<2*ta_.blockSize(); ++i)
    ensure_equals("invalid padding byte", size_t(ta_.last_[i]), ta_.blockSize() );
}

// test small-input
template<>
template<>
void testObj::test<2>(void)
{
  const char text[] = "test str";
  // fill with data
  for(const char *it = text; *it!=0; ++it)
    data_.push_back(*it);
  data_.push_back('\0');

  ta_.encrypt(data_);
  // check total size
  ensure_equals("invalid number of bytes", ta_.last_.size(), 2*ta_.blockSize() );
  // ensure proper padding
  for(size_t i=ta_.blockSize()+Util::tabSize(text); i<2*ta_.blockSize(); ++i)
    ensure_equals("invalid padding byte", size_t(ta_.last_[i]), ta_.blockSize()-Util::tabSize(text) );
}

// test long input
template<>
template<>
void testObj::test<3>(void)
{
  const char text[] = "test string 2";
  // fill with data
  for(const char *it = text; *it!=0; ++it)
    data_.push_back(*it);
  data_.push_back('\0');

  ta_.encrypt(data_);
  // check total size
  ensure_equals("invalid number of bytes", ta_.last_.size(), 3*ta_.blockSize() );
  // ensure proper padding
  for(size_t i=2*ta_.blockSize()+Util::tabSize(text); i<3*ta_.blockSize(); ++i)
    ensure_equals("invalid padding byte", size_t(ta_.last_[i]), 2*ta_.blockSize()-Util::tabSize(text) );
}

// test small-input decryption
template<>
template<>
void testObj::test<4>(void)
{
  // random junk at the begining
  for(size_t i=0; i<ta_.blockSize(); ++i)
    data_.push_back('!');
  // copy message
  const char text[] = "short";
  for(size_t i=0; i<Util::tabSize(text)-1; ++i)
    data_.push_back(text[i]);
  // add padding of short message
  for(size_t i=Util::tabSize(text)-1; i<ta_.blockSize(); ++i)
    data_.push_back( ta_.blockSize()-Util::tabSize(text)+1 );
  // decrypt
  ta_.decrypt(data_);
  // test output
  std::string out;
  std::copy( data_.begin(), data_.end(), std::back_insert_iterator<std::string>(out) );
  ensure_equals("invalid decyphered string", out, text);
}

// test long input
template<>
template<>
void testObj::test<5>(void)
{
  // random junk at the begining
  for(size_t i=0; i<ta_.blockSize(); ++i)
    data_.push_back('!');
  // copy message
  const char text[] = "not so short";
  for(size_t i=0; i<Util::tabSize(text)-1; ++i)
    data_.push_back(text[i]);
  // add padding of message
  for(size_t i=Util::tabSize(text)-1; i<2*ta_.blockSize(); ++i)
    data_.push_back( 2*ta_.blockSize()-Util::tabSize(text)+1 );
  // decrypt
  ta_.decrypt(data_);
  // test output
  std::string out;
  std::copy( data_.begin(), data_.end(), std::back_insert_iterator<std::string>(out) );
  ensure_equals("invalid decyphered string", out, text);
}

// test too-small decryption buffer
template<>
template<>
void testObj::test<6>(void)
{
  data_.push_back(42);
  try
  {
    ta_.decrypt(data_);
    fail("no exception on too small buffer");
  }
  catch(const std::runtime_error&)
  { }
}

// test invalid size
template<>
template<>
void testObj::test<7>(void)
{
  for(size_t i=0; i<2*ta_.blockSize()+1; ++i)
    data_.push_back(42);
  try
  {
    ta_.decrypt(data_);
    fail("no exception on invalid size of the buffer");
  }
  catch(const std::runtime_error&)
  { }
}

// test empty-input decryption
template<>
template<>
void testObj::test<8>(void)
{
  // random junk at the begining
  for(size_t i=0; i<ta_.blockSize(); ++i)
    data_.push_back(i);
  // add padding of empty message
  for(size_t i=0; i<ta_.blockSize(); ++i)
    data_.push_back(ta_.blockSize());

  ta_.decrypt(data_);
  ensure_equals("invalid buffer size", data_.size(), 0);
}

} // namespace tut
