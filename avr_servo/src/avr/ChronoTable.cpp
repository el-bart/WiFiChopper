#include "config.hpp"

#include "ChronoTable.hpp"
#include "bubbleSort.hpp"
#include "servoNoToMask.hpp"
#include "uassert.hpp"


ChronoTable::ChronoTable(const Positions &defaultPositions)
{
  // initially start with default values
  cur_=defaultPositions;
  // compute and update table for given entries
  update();
}


void ChronoTable::update(void)
{
  uint8_t idxs[SERVO_COUNT]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // sorted indexes
  bubbleSort(idxs, cur_);                                               // find proper order of elements

  uint8_t idxOut=0;
  for(uint8_t i=0; i<SERVO_COUNT; ++i)
  {
    // make default mask for this element
    uassert(idxs[i]<SERVO_COUNT);
    Entry entry={cur_[idxs[i]], 0xFF, 0xFF, 0xFF};
    servoNoToMask(idxs[i], entry);
    // try packing multiple entries into one, if possible
    while( i<SERVO_COUNT-1 && cur_[idxs[i]]==cur_[idxs[i+1]] )
    {
      ++i;
      servoNoToMask(idxs[i], entry);
    }
    // save this entry
    uassert(idxOut<SERVO_COUNT);
    e_[idxOut]=entry;
    ++idxOut;
  }

  // fill up missing elements with zeros
  const Entry zero={0, 0x00, 0x00, 0x00};
  for(uint8_t i=idxOut; i<SERVO_COUNT; ++i)
    e_[i]=zero;
}
