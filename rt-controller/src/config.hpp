#ifndef INCLUDE_CONFIG_HPP_FILE
#define INCLUDE_CONFIG_HPP_FILE

// quarc frequency in [Hz] (note: this must be define...)
#define F_CPU (8L*1000L*1000L)

// now system files can be safely included
#include <inttypes.h>


// USART speed (bps)
//constexpr uint16_t USART_BAUD=19200L;
constexpr uint16_t USART_BAUD=38400L;

// version number: main, major, minor
constexpr uint8_t VERSION[3]={0,0,0};
// sanity checks:
static_assert( VERSION[0]<10, "number too big" );
static_assert( VERSION[1]<10, "number too big" );
static_assert( VERSION[2]<10, "number too big" );

// input text buffer length
constexpr uint8_t INPUT_BUF_SIZE=16;

#endif
