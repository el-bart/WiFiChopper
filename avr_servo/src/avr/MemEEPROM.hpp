#ifndef INCLUDE_MEMEEPROM_HPP_FILE
#define INCLUDE_MEMEEPROM_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include <avr/eeprom.h>

// TODO: make this code custom, using interrupts, so that EEPROM writes will be non-blocking.

/** \brief access facade for EEPROM memory.
 *  \note typical EEPROM write takes ~8.5ms. this means it cannot be
 *        done w-out interrupts, so that it will not interferre with
 *        servos controll. for now - don't use this in operational mode!
 */
class MemEEPROM
{
public:
  /** \brief reads data from eeprom.
   *  \param p position to read from.
   *  \return servo's entry.
   */
  const uint8_t read(const uint8_t p) const
  {
    return eeprom_read_byte(toAddr(p));
  }

  /** \brief writes data to eeprom.
   *  \param p position to write to.
   *  \param e entry to be written.
   */
  void write(const uint8_t p, const uint8_t d)
  {
    eeprom_update_byte(toAddr(p), d);
    // wait for the write to finish
    //eeprom_busy_wait();
  }

private:
  uint8_t *toAddr(uint8_t num) const
  {
    return static_cast<uint8_t*>(0)+num;
  }
}; // class MemEEPROM

#endif
