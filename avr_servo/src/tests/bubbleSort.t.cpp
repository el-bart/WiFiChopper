#include <tut/tut.hpp>
#include <vector>
#include <algorithm>

#include "bubbleSort.hpp"

namespace
{
struct TestClass
{
  TestClass(void)
  {
    for(size_t i=0; i<sizeof(idxs_)/sizeof(idxs_[0]); ++i)
      idxs_[i]=i;
  }

  void testSort(uint8_t e1, uint8_t e2, uint8_t e3, uint8_t e4)
  {
    // sort using custom algo
    p_[0]=e1;
    p_[1]=e2;
    p_[2]=e3;
    p_[3]=e4;
    bubbleSort(idxs_, p_);

    // sort with reference implementation
    std::vector<int> v;
    for(int i=0; i<4; ++i)
      v.push_back(p_[i]);
    sort(v.begin(), v.end());

    // check
    for(int i=0; i<4; ++i)
      tut::ensure_equals("unexpected element", (int)p_[idxs_[i]], v[i]);
  }

  Positions p_;
  uint8_t   idxs_[4];
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("bubbleSort");
} // unnamed namespace


namespace tut
{

// test sorted table
template<>
template<>
void testObj::test<1>(void)
{
  testSort(10,20,30,40);
}

// test reversly sorted table
template<>
template<>
void testObj::test<2>(void)
{
  testSort(40,30,20,10);
}

// test random data set
template<>
template<>
void testObj::test<3>(void)
{
  testSort(20,30,40,10);
}

// test data set with repeating elements
template<>
template<>
void testObj::test<4>(void)
{
  testSort(30,30,10,10);
}

} // namespace tut
