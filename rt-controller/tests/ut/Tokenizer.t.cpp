#include <tut/tut.hpp>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cstring>

#include "Tokenizer.hpp"

using namespace std;

namespace
{
struct TestClass
{
  void testTok(const string in, const vector<string> out) const
  {
    // make writable copy
    unique_ptr<char[]> inPtr(new char[in.length()+1]);
    strcpy(inPtr.get(), in.c_str());
    // tokenize
    Tokenizer    tok(inPtr.get());
    const size_t count=0;
    // check tokens
    for(const string &e: out)
    {
      const char *tmp=tok.getNextToken();
      if(tmp==nullptr)
      {
        stringstream ss;
        ss<<"got NULL token on position "<<count;
        tut::fail( ss.str().c_str() );
      }
      stringstream ss;
      ss<<"invalid token number "<<count;
      tut::ensure_equals(ss.str().c_str(), tmp, e);
    }
    // ensure there is exact number of tokens
    tut::ensure("too many tokens found", tok.getNextToken()==nullptr);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Tokenizer");
} // unnamed namespace


namespace tut
{

// tokenize empty string
template<>
template<>
void testObj::test<1>(void)
{
  testTok("", {});
}

// tokenize separators-only set
template<>
template<>
void testObj::test<2>(void)
{
  testTok("  \t \t ", {});
}

// tokenize single string
template<>
template<>
void testObj::test<3>(void)
{
  testTok("abc", {"abc"});
}

// try tokenizing multiple tokens
template<>
template<>
void testObj::test<4>(void)
{
  testTok("abc def", {"abc", "def"});
}

// test NULL
template<>
template<>
void testObj::test<5>(void)
{
  Tokenizer t(nullptr);
  ensure("tokens in NULL string found", t.getNextToken()==nullptr);
}

// test many tokens
template<>
template<>
void testObj::test<6>(void)
{
  testTok("alice has a cat", {"alice", "has", "a", "cat"});
}

// test multiple separators between tokens
template<>
template<>
void testObj::test<7>(void)
{
  testTok(" \t  abc \t  def  \t", {"abc", "def"});
}

// test trailing separators
template<>
template<>
void testObj::test<8>(void)
{
  testTok("abc  \t ", {"abc"});
}

} // namespace tut
