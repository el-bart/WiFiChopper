#ifndef INCLUDE_TABLEEEPROM_HPP_FILE
#define INCLUDE_TABLEEEPROM_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "MemEEPROM.hpp"
//#include "uassert.hpp"

/** \brief table of given elements settings, kept in EEPROM
 */
class TableEEPROM
{
public:
  explicit TableEEPROM(uint8_t base):
    base_(base)
  {
  }

  /** \brief read data from table.
   *  \param n element number to access.
   *  \return servo's entry.
   */
  const uint8_t read(const uint8_t n) const
  {
    //uassert(n<N);
    return eeprom_.read(base_+n);
  }

  /** \brief write data to the table.
   *  \param n element number to access.
   *  \param d data to be written.
   */
  void write(const uint8_t n, const uint8_t d)
  {
    //uassert(n<N);
    eeprom_.write(base_+n, d);
  }

private:
  uint8_t   base_;
  MemEEPROM eeprom_;
}; // class TableEEPROM

#endif
