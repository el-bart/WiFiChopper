#ifndef INCLUDE_PERSISTENTTABLE_HPP_FILE
#define INCLUDE_PERSISTENTTABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "Table.hpp"
#include "TableEEPROM.hpp"
#include "uassert.hpp"

/** \brief table keept in memory (for fast access) and in EEPROM (for persistnecy).
 */
template<uint8_t N>
class PersistentTable
{
private:
  typedef Table<uint8_t, N> TableRam;

public:
  explicit PersistentTable(uint8_t base):
    eeprom_(base)
  {
    // read initial values from EEPROM
    for(uint8_t i=0; i<N; ++i)
      ram_[i]=eeprom_.read(i);
  }

  /** \brief read data from table.
   *  \param n element number to access.
   *  \return servo's entry.
   */
  uint8_t read(const uint8_t n) const
  {
    uassert(n<N);
    return ram_[n];
  }

  /** \brief write data to the table.
   *  \param n element number to access.
   *  \param d data to be written.
   */
  void write(const uint8_t n, const uint8_t d)
  {
    uassert(n<N);
    ram_[n]=d;
    eeprom_.write(n, d);
  }

private:
  TableRam    ram_;
  TableEEPROM eeprom_;
}; // class PersistentTable

#endif
