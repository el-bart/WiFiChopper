#ifndef INCLUDE_PHASEGENERATOR_HPP_FILE
#define INCLUDE_PHASEGENERATOR_HPP_FILE

#include "config.hpp"
#include "Timer1.hpp"
#include "EntryTable.hpp"
#include "Noncopyable.hpp"

class PhaseGenerator: private Noncopyable
{
public:
  PhaseGenerator(void);

  void rise(void) const;
  void generate(Timer1 &t1, const EntryTable &et) const;
  void fall(void) const;

private:
  const uint8_t defMaskPB_;     // mask to apply to all entries of port B
  const uint8_t defMaskPC_;     // mask to apply to all entries of port C
  const uint8_t defMaskPD_;     // mask to apply to all entries of port D
}; // class PhaseGenerator

#endif
