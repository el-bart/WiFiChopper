#include "config.hpp"

#include <avr/pgmspace.h>

#include "Processor.hpp"
#include "USART.hpp"
#include "utils.hpp"
#include "CommLostProtector.hpp"


namespace
{
// some commonly used flash strings are here:
const char g_tooManyArgsStr[]  PROGMEM = "too many args";
const char g_missingArgsStr[]  PROGMEM = "missing arg(s)";
const char g_invalidHexValue[] PROGMEM = "invalid hex value";


// converts char to value, as hex
bool char2hex(uint8_t &h, const char c)
{
  if('0'<=c && c<='9')
  {
    h=c-'0';
    return true;
  }
  if('a'<=c && c<='f')
  {
    h=10+(c-'a');
    return true;
  }
  return false;
}


// parses given string as a hex value
bool parseAsHex(uint8_t &out, const char *str)
{
  // sanity check
  if(str==nullptr)
    return false;

  uint8_t bytes[2];
  // check first byte
  if( !char2hex(bytes[0], str[0]) )
    return false;
  // check second byte
  if( !char2hex(bytes[1], str[1]) )
    return false;
  // ensure there is no more data
  if( str[2]!=0 )
    return false;
  // combine response
  out = (bytes[0]<<4) | bytes[1];
  return true;
}


// sends given number as hex value
void sendAsHex(const uint8_t n)
{
  const uint8_t bytes[2] = { static_cast<uint8_t>(n>>4), static_cast<uint8_t>(n&0x0F) };
  for(auto b: bytes)
    USART::send( "0123456789abcdef"[b] );
}


// s1 - string in RAM, s2 - string in FLASH
bool strEqRF(const char* s1, const char* s2)
{
  // both nulls?
  if(s1==nullptr && s2==nullptr)
    return true;

  // check each char...
  while(true)
  {
    const char c2=pgm_read_byte(s2);
    // end of string?
    if(*s1==0 && c2==0)
      return true;
    // non-equal characters?
    if(*s1!=c2)
      return false;
    // one of the strings is shorter?
    if(*s1==0 || c2==0)
      return false;
    // move on to the next character
    ++s1;
    ++s2;
  }
} // strEq()

} // unnamed namespace



void Processor::process(char *buf, const AdcReadsWrapper::Reads &reads)
{
  // tokenize input
  Tokenizer tokenizer(buf);

  // sanity check
  const char *cmd=tokenizer.getNextToken();
  if(cmd==nullptr)
  {
    errorFlash( PSTR("no cmd") );
    return;
  }

  if( strEqRF(cmd, PSTR("hello")) )
  {
    handleHello(tokenizer);
    return;
  }

  if( strEqRF(cmd, PSTR("accel?")) )
  {
    handleAccel(tokenizer, reads);
    return;
  }

  if( strEqRF(cmd, PSTR("eng?")) )
  {
    handleEng(tokenizer);
    return;
  }

  if( strEqRF(cmd, PSTR("engset")) )
  {
    handleEngSet(tokenizer);
    return;
  }

  if( strEqRF(cmd, PSTR("vin?")) )
  {
    handleVin(tokenizer, reads);
    return;
  }

  if( strEqRF(cmd, PSTR("enableclp")) )
  {
    handleEnableCLP(tokenizer);
    return;
  }

  errorFlash( PSTR("unknown cmd") );
}


void Processor::errorFlash(const char *str)
{
  USART::sendFlash( PSTR("ERROR: ") );
  if(str!=nullptr)
    USART::sendFlash(str);
  USART::send('\n');
}


void Processor::handleHello(Tokenizer& tokenizer)
{
  if( tokenizer.getNextToken()!=nullptr )
  {
    errorFlash(g_tooManyArgsStr);
    return;
  }
  // send the response
  USART::sendFlash( PSTR("WiFiChopper RT-board v") );
  // send version numbers
  USART::send('0'+VERSION[0]);
  USART::send('.');
  USART::send('0'+VERSION[1]);
  USART::send('.');
  USART::send('0'+VERSION[2]);
  USART::send('\n');
}


void Processor::handleAccel(Tokenizer& tokenizer, const AdcReadsWrapper::Reads &reads)
{
  // sanity checks
  if( tokenizer.getNextToken()!=nullptr )
  {
    errorFlash(g_tooManyArgsStr);
    return;
  }
  // send reading from each axis
  sendAsHex(reads.ox_);
  USART::send(' ');
  sendAsHex(reads.oy_);
  USART::send(' ');
  sendAsHex(reads.oz_);
  USART::send('\n');
}


void Processor::handleEng(Tokenizer& tokenizer)
{
  // sanity checks
  if( tokenizer.getNextToken()!=nullptr )
  {
    errorFlash(g_tooManyArgsStr);
    return;
  }

  // read engines speeds and send back to the host

  // main engine 1
  sendAsHex( eng_.getMain1() );
  USART::send(' ');
  // main engine 2
  sendAsHex( eng_.getMain2() );
  USART::send(' ');
  // rear engine
  const EngSpeed::RearEng rear=eng_.getRear();
  USART::send( rear.forward_?'+':'-' );
  sendAsHex(rear.value_);
  // eol
  USART::send('\n');
}


void Processor::handleEngSet(Tokenizer& tokenizer)
{
  // sanity checks
  const char *main1Str = tokenizer.getNextToken();
  if(main1Str==nullptr)
  {
    errorFlash(g_missingArgsStr);
    return;
  }
  const char *main2Str = tokenizer.getNextToken();
  if(main2Str==nullptr)
  {
    errorFlash(g_missingArgsStr);
    return;
  }
  const char *rearStr = tokenizer.getNextToken();
  if(rearStr==nullptr)
  {
    errorFlash(g_missingArgsStr);
    return;
  }
  // nothing more should be present
  if( tokenizer.getNextToken()!=nullptr )
  {
    errorFlash(g_tooManyArgsStr);
    return;
  }

  // check input values

  // convert main1
  uint8_t main1;
  if( !parseAsHex(main1, main1Str) )
  {
    errorFlash(g_invalidHexValue);
    return;
  }
  // convert main2
  uint8_t main2;
  if( !parseAsHex(main2, main2Str) )
  {
    errorFlash(g_invalidHexValue);
    return;
  }
  // convert rear dir
  bool rearForward;
  switch(rearStr[0])
  {
    case '+': rearForward=true;  break;
    case '-': rearForward=false; break;
    default: errorFlash( PSTR("invalid sign") ); return;
  } // switch(rear/sign)
  // convert rear value
  uint8_t rearValue;
  if( !parseAsHex(rearValue, rearStr+1) )
  {
    errorFlash(g_invalidHexValue);
    return;
  }

  // setting new engines values
  eng_.setMain1(main1);
  eng_.setMain2(main2);
  eng_.setRear( EngSpeed::RearEng{rearForward, rearValue} );

  // sending response
  USART::sendFlash( PSTR("set") );
  sendAsHex(main1);
  sendAsHex(main2);
  USART::send( rearForward?'+':'-' );
  sendAsHex(rearValue);
  USART::send('\n');
}


void Processor::handleVin(Tokenizer& tokenizer, const AdcReadsWrapper::Reads &reads)
{
  // sanity checks
  if( tokenizer.getNextToken()!=nullptr )
  {
    errorFlash(g_tooManyArgsStr);
    return;
  }
  // send input voltage reading
  sendAsHex(reads.vin_);
  USART::send('\n');
}


void Processor::handleEnableCLP(Tokenizer& tokenizer)
{
  // sanity checks
  if( tokenizer.getNextToken()!=nullptr )
  {
    errorFlash(g_tooManyArgsStr);
    return;
  }
  CommLostProtector::enable();
  USART::sendFlash( PSTR("enabled\n") );
}
