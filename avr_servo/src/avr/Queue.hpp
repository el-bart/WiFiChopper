#ifndef INCLUDE_QUEUE_HPP_FILE
#define INCLUDE_QUEUE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "ScopedIntLock.hpp"


template<uint8_t N>
class Queue
{
public:
  Queue(void):
    begin_(0),
    size_(0)
  {
  }

  void push(uint8_t b)
  {
    ScopedIntLock lock;
    const uint8_t idx=(begin_+size())%N;
    d_[idx]=b;
    if(size()!=N)
      ++size_;
    else
      ++begin_;
  }

  uint8_t peek(uint8_t pos) const
  {
    ScopedIntLock lock;
    const uint8_t idx=(begin_+pos)%N;
    return d_[idx];
  }

  uint8_t pop(void)
  {
    ScopedIntLock lock;
    const uint8_t tmp=d_[begin_];
    remove(1);
    return tmp;
  }

  void remove(const uint8_t count)
  {
    ScopedIntLock lock;
    if(count>=size())
    {
      clear();
      return;
    }
    begin_=(begin_+count)%N;
    size_-=count;
  }

  void clear(void)
  {
    ScopedIntLock lock;
    size_ =0;
    begin_=0;
  }

  uint8_t size(void) const
  {
    ScopedIntLock lock;
    return size_;
  }

private:
  volatile uint8_t begin_;
  volatile uint8_t size_;
  volatile uint8_t d_[N];
}; // class Queue

#endif
