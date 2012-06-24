#ifndef INCLUDE_FASTQUEUE_HPP_FILE
#define INCLUDE_FASTQUEUE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "uassert.hpp"


template<uint8_t N>
class FastQueue
{
public:
  FastQueue(void)
  {
    clear();
  }

  void push(uint8_t b)
  {
    uassert(end_<N);
    d_[end_]=b;
    ++end_;
  }

  uint8_t peek(uint8_t pos) const
  {
    uassert(begin_+pos<N);
    return d_[begin_+pos];
  }

  uint8_t pop(void)
  {
    uassert(begin_<N);
    const uint8_t tmp=d_[begin_];
    ++begin_;
    return tmp;
  }

  void clear(void)
  {
    end_  =0;
    begin_=0;
    uassert(size()==0);
  }

  uint8_t size(void) const
  {
    uassert(end_>=begin_);
    return end_-begin_;
  }

private:
  volatile uint8_t begin_;
  volatile uint8_t end_;
  volatile uint8_t d_[N];
}; // class Queue

#endif
