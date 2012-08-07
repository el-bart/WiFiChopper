#include "config.hpp"
#include <ctype.h>

#include "CommProtocol.hpp"
#include "nameToNumber.hpp"
#include "uassert.hpp"

CommProtocol::CommProtocol(QueueSend &qSend, QueueRecv &qRecv, PersistentSettings &settings):
  qSend_(qSend),
  qRecv_(qRecv),
  settings_(settings)
{
}


namespace
{
uint8_t hex2num(char h)
{
  // digit?
  if('0'<=h && h<='9')
    return h-'0';
  // letter?
  h=tolower(h);
  if('a'<=h && h<='f')
    return 10+(h-'a');
  // error..
  return 0xFF;
} // hex2num()


char num2hex(uint8_t n)
{
  if(n>0x0F)
    return '?';
  const char *lut="0123456789abcdef";
  return lut[n];
} // num2hex()


char computeChecksum(const char servoName, const char mode, const char posH, const char posL)
{
  // compute XOR of all fields
  uint8_t out=0x00;
  out^=servoName;
  out^=mode;
  out^=posH;
  out^=posL;
  // compute xor of half-bytes
  out=((out>>4)&0x0F)^(out&0x0F);
  // return as printable hex
  return num2hex(out);
} // computeChecksum()


inline bool isNewLine(const char c)
{
  return c=='\n' || c=='\r';
} // isNewLine()


bool hasNewPacket(const QueueRecv &q)
{
  uint8_t p=0;
  for(; p<q.size(); ++p)
    if( isNewLine( q.peek(p) ) )
      return true;
  return false;
} // has NewPacket()
} // unnamed namespace


void CommProtocol::process(Positions &posTab)
{
  while( hasNewPacket(qRecv_) )
  {
    uassert( qRecv_.size()>0 );

    // get servo number
    const char servoName=qRecv_.pop();
    if(servoName<'a' || 'a'+SERVO_COUNT<=servoName)
    {
      replyError('?');
      skipUntilNewCommand();
      continue;
    }
    const uint8_t servoNo=nameToNumber(servoName);
    uassert(servoNo<SERVO_COUNT);

    // get mode
    const char mode=qRecv_.pop();
    if(mode!='l' && mode!='h' && mode!='d' && mode!='s')
    {
      replyError(servoName);
      skipUntilNewCommand();
      continue;
    }

    // get position
    const char    posHexHigh=qRecv_.pop();
    const uint8_t posHigh   =hex2num(posHexHigh);
    if(posHigh>0x0F)
    {
      replyError(servoName);
      skipUntilNewCommand();
      continue;
    }
    const char    posHexLow=qRecv_.pop();
    const uint8_t posLow   =hex2num(posHexLow);
    if(posLow>0x0F)
    {
      replyError(servoName);
      skipUntilNewCommand();
      continue;
    }
    const uint8_t pos=posHigh*16+posLow;

    // test checksum correctness
    const char checksumRecv=qRecv_.pop();
    const char checksumComp=computeChecksum(servoName, mode, posHexHigh, posHexLow);
    if(checksumRecv!=checksumComp && checksumRecv!='?')
    {
      replyError(servoName);
      skipUntilNewCommand();
      continue;
    }

    // check for end of line
    const char end=qRecv_.pop();
    if( !isNewLine(end) )
    {
      replyError(servoName);
      skipUntilNewCommand();
      continue;
    }

    // remove extra elements from the queue
    skipEndMarkers();

    // execute received orders
    const bool ret=execute(servoNo, mode, pos, posTab);
    // send confirmation, if it is ok or not
    if(ret)
      replyOk(servoName);
    else
      replyError(servoName);
  } // while(has_more_date)
}


void CommProtocol::skipUntilNewCommand(void)
{
  // remove characters until end of command
  uint8_t toRemove=0;
  for(uint8_t i=0; i<qRecv_.size(); ++i)
  {
    const char c=qRecv_.peek(i);
    if( isNewLine(c) )
      break;
    ++toRemove;
  }
  qRecv_.remove(toRemove);

  // now skip all end markers
  skipEndMarkers();
}


void CommProtocol::skipEndMarkers(void)
{
  uint8_t toRemove=0;
  for(uint8_t i=0; i<qRecv_.size(); ++i)
  {
    const char c=qRecv_.peek(i);
    if( !isNewLine(c) )
      break;
    ++toRemove;
  }
  qRecv_.remove(toRemove);
}


bool CommProtocol::execute(const uint8_t srvNo, const char mode, const uint8_t pos, Positions &posTab)
{
  uassert(srvNo<SERVO_COUNT);
  // read these to make code shorter later on
  const uint8_t pMin=settings_.posMin().read(srvNo);
  const uint8_t pMax=settings_.posMax().read(srvNo);
  uint8_t       tmp =pos;
  // check mode
  switch(mode)
  {
    case 's':   // set servo to given position
         if(tmp<pMin)
           tmp=pMin;
         else
           if(tmp>pMax)
             tmp=pMax;
         posTab[srvNo]=tmp;
         return true;

    case 'l':   // set minimum value
         if(tmp>pMax)
           tmp=pMax;
         settings_.posMin().write(srvNo, tmp);
         return true;

    case 'h':   // set maximum value
         if(tmp<pMin)
           tmp=pMin;
         settings_.posMax().write(srvNo, tmp);
         return true;

    case 'd':   // set default value
         if(tmp<pMin)
           tmp=pMin;
         else
           if(tmp>pMax)
             tmp=pMax;
         settings_.posDef().write(srvNo, tmp);
         return true;
  } // switch(mode)

  // unknown mode...
  return false;
}


void CommProtocol::replyOk(const char srvName)
{
  replyStr(srvName, "-ok\n");
}


void CommProtocol::replyError(const char srvName)
{
  replyStr(srvName, "-ERR\n");
}

void CommProtocol::replyStr(const char srvName, const char *str)
{
  qSend_.push(srvName);
  for(const char *it=str; *it!=0; ++it)
    qSend_.push(*it);
}
