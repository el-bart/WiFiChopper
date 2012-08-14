#include <tut/tut.hpp>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "Util/hex.hpp"

using namespace Util;

namespace
{
struct TestClass
{
  void testEq(const std::vector<uint8_t> &out, const std::string &expected) const
  {
    std::string str;
    std::copy( out.begin(), out.end(), std::back_insert_iterator<std::string>(str) );
    tut::ensure_equals("invalid string", str, expected);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/hex");
} // unnamed namespace


namespace tut
{

// test empty string to hex
template<>
template<>
void testObj::test<1>(void)
{
  std::vector<uint8_t> in;
  auto out = toHex(in);
  ensure_equals("invalid hex string", out, "");
}

// test random data to hex
template<>
template<>
void testObj::test<2>(void)
{
  auto out = toHex( {0x40, 0x13, 0xbF} );
  ensure_equals("invalid hex string", out, "4013bf");
}

// test empty string to data
template<>
template<>
void testObj::test<3>(void)
{
  auto out = fromHex("");
  testEq(out, "");
}

// test non-empty string to data
template<>
template<>
void testObj::test<4>(void)
{
  auto out = fromHex("414243");
  testEq(out, "ABC");
}

// test exception on non-hex char
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    fromHex("aX");
    fail("no error on invalid char");
  }
  catch(const std::runtime_error&)
  { }
}

// test exception on invalid number of characters
template<>
template<>
void testObj::test<6>(void)
{
  try
  {
    fromHex("aaf");
    fail("no error on invalid number of characters");
  }
  catch(const std::runtime_error&)
  { }
}

// test parsing all hex digits
template<>
template<>
void testObj::test<7>(void)
{
  auto out = fromHex("404142434445464748494a4B4c4D4e4F");
  testEq(out, "@ABCDEFGHIJKLMNO");
}

// test all to-hex parsing
template<>
template<>
void testObj::test<8>(void)
{
  auto out = toHex( {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
                     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f} );
  ensure_equals("invalid hex string", out, "00102030405060708090a0b0c0d0e0f0"
                                           "000102030405060708090a0b0c0d0e0f");
}

// test parsing number
template<>
template<>
void testObj::test<9>(void)
{
  const auto p = toHex(0xF9);
  ensure_equals("invalid hex 1", p.first,  'f');
  ensure_equals("invalid hex 2", p.second, '9');
}

// test parsing single hex
template<>
template<>
void testObj::test<10>(void)
{
  const auto p = fromHex('e');
  ensure_equals("invalid number parsed", p, 0xE);
}

} // namespace tut
