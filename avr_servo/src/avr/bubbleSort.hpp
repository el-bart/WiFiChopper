#ifndef INCLUDE_BUBBLESORT_HPP_FILE
#define INCLUDE_BUBBLESORT_HPP_FILE

#include "config.hpp"

#include "Positions.hpp"

template<uint8_t N>
inline void bubbleSort(uint8_t (&idxs)[N], const Positions &c)
{
  // find proper order of elements
  for(uint8_t i=0; i<N; ++i)
  {
    bool changed=false;
    for(uint8_t j=1; j<N; ++j)
    {
      if( c[idxs[j-1]] > c[idxs[j]] )
      {
        // swap
        const uint8_t tmp=idxs[j-1];
        idxs[j-1]=idxs[j];
        idxs[j]  =tmp;
        // mark some change
        changed=true;
      }
    }
    // already sorted?
    if(!changed)
      break;
  }
} // bubbleSort()

#endif
