#ifndef INCLUDE_ENTRYTABLE_HPP_FILE
#define INCLUDE_ENTRYTABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "Table.hpp"

/** \brief entry of a single processing data unit.
 */
struct Entry
{
  uint8_t time_;        ///< time to switch pin(s) off
  uint8_t maskPB_;      ///< mask of pins to be turned off in port B
  uint8_t maskPC_;      ///< mask of pins to be turned off in port C
  uint8_t maskPD_;      ///< mask of pins to be turned off in port D
}; // struct Entry

typedef Table<Entry, SERVO_COUNT> EntryTable;

#endif
