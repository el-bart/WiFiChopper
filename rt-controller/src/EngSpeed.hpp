#ifndef INCLUDE_ENGSPEED_HPP_FILE
#define INCLUDE_ENGSPEED_HPP_FILE

#include "config.hpp"

#include <inttypes.h>

/** \brief control mechanism for the engines.
 */
class EngSpeed
{
public:
  EngSpeed(void);

  void stop(void);

  uint8_t getMain1(void) const;
  void setMain1(uint8_t v);

  uint8_t getMain2(void) const;
  void setMain2(uint8_t v);

  struct RearEng
  {
    bool    forward_;
    uint8_t value_;
  };

  RearEng getRear(void) const;
  void setRear(RearEng e);
}; // class EngSpeed

#endif
