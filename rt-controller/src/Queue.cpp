/*
 * Queue.cpp
 *
 */
#include <util/atomic.h>

#include "Queue.hpp"
#include "uassert.hpp"

Queue::Queue(void):
  begin_(0u),
  size_(0u)
{
}

void Queue::push(uint8_t b)
{
  uassert( !full() );
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    size_t pos=begin_+size_;
    if(pos>=QUEUE_MAX_SIZE)
      pos-=QUEUE_MAX_SIZE;
    queue_[pos]=b;
    ++size_;
  }
}

size_t Queue::size(void) const
{
  size_t tmp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    tmp=size_;
  }
  uassert(tmp<=QUEUE_MAX_SIZE);
  return tmp;
}

uint8_t Queue::peek(size_t pos) const
{
  uint8_t tmp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    uassert(size_>pos);
    size_t qpos=begin_+pos;
    if(qpos>=QUEUE_MAX_SIZE)
      qpos-=QUEUE_MAX_SIZE;
    tmp=queue_[qpos];
  }
  return tmp;
}

uint8_t Queue::pop(void)
{
  uassert( !empty() );
  uint8_t tmp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    tmp=queue_[begin_];
    // move the begining
    ++begin_;
    if(begin_==QUEUE_MAX_SIZE)
      begin_=0;
    // update size
    --size_;
  }
  return tmp;
}

void Queue::popCount(size_t n)
{
  uassert( size()>=n );
  for(size_t i=0; i<n; ++i)
    pop();
}
