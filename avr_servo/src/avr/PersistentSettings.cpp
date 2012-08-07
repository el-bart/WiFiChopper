#include "config.hpp"
#include "PersistentSettings.hpp"
#include "uassert.hpp"

PersistentSettings::PersistentSettings(void):
  // NOTE: AVRs have a bug, that when power is down while writing to EEPROM
  //       contents of the first byte may change. to avoid this, it must no
  //       be used inside the program
  posMin_(1+0*SERVO_COUNT),
  posDef_(1+1*SERVO_COUNT),
  posMax_(1+2*SERVO_COUNT),
  flags_ (1+3*SERVO_COUNT)
{
  if(flags_.read(0)==flags_.read(1))
  {
    writeDefaultValues();
    flags_.write(0, 0x42);      // just mark that something has changed
  }
  uassert(flags_.read(0)!=flags_.read(1));
}

void PersistentSettings::writeDefaultValues(void)
{
  for(int i=0; i<SERVO_COUNT; ++i)
  {
    posMin_.write(i, 0x00);
    posDef_.write(i, 0xFF/2);
    posMax_.write(i, 0xFF);
  }
}
