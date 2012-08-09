#include <tut/tut.hpp>
#include <iterator>
#include <string>
#include <cassert>

#include "Crypto/Aes256.hpp"

using namespace Crypto;

namespace
{
struct TestClass
{
  TestClass(void):
    aes_( makeKey(), makeIV() ),
    api_(aes_)
  { }

  BinData makeKey(void) const
  {
    BinData tmp( Aes256::keySize() );
    for(size_t i=0; i<tmp.size(); ++i)
      tmp.data()[i]=i;
    return std::move(tmp);
  }

  BinData makeIV(void) const
  {
    BinData tmp( Aes256::ivSize() );
    for(size_t i=0; i<tmp.size(); ++i)
      tmp.data()[i]=0x42;
    return std::move(tmp);
  }

  Aes256   aes_;
  Algo    &api_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/Aes256");
} // unnamed namespace


namespace tut
{

// test getting key and IV sizes
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid key size", Aes256::keySize(), 256/8);
  ensure_equals("invalid IV size",  Aes256::ivSize(),  256/8);
}

// test name, mode and block size
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid name", api_.name(), std::string(MCRYPT_RIJNDAEL_256) );
  ensure_equals("invalid mode", api_.mode(), std::string(MCRYPT_CFB) );
  ensure_equals("invalid block size", api_.blockSize(), 256/8);
}

// test encryption and decryption
template<>
template<>
void testObj::test<3>(void)
{
  const char text[] = "alice has a cypher";
  assert( (sizeof(text) < Aes256::keySize()) && "test text is too long" );

  // copy text to vector
  Algo::Data data;
  for(const char* str=text; *str!=0; ++str)
    data.push_back(*str);

  // encrypt
  api_.encrypt(data);

  // check if it is not plaintext
  {
    std::string tmp;
    copy( data.begin(), data.end(), std::back_insert_iterator<std::string>(tmp) );
    ensure("plaintext detected!", tmp.c_str()!=std::string(text) ); // NOTE: tmp has trailing '0' that makes != fail to detect difference
  }

  // decrypt
  Aes256 dec( makeKey(), makeIV() );
  dec.decrypt(data);

  // check decrypted message
  {
    std::string tmp;
    copy( data.begin(), data.end(), std::back_insert_iterator<std::string>(tmp) );
    ensure_equals("decryption error", tmp.c_str(), std::string(text) ); // NOTE: tmp has trailing '0' that makes != fail to detect difference
  }
}

} // namespace tut
