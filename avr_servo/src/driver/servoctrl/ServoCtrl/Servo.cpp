#include "ServoCtrl/Servo.hpp"

namespace ServoCtrl
{

Servo::Servo(ServoName name, CommDevicePtrNN dev_):
  name_{ std::move(name) },
  dev_{ std::move(dev_) }
{
}

Response Servo::setPos(uint8_t pos)
{
  return send('s', pos);
}

Response Servo::setMinPos(uint8_t pos)
{
  return send('l', pos);
}

Response Servo::setMaxPos(uint8_t pos)
{
  return send('h', pos);
}

Response Servo::setDefaultPos(uint8_t pos)
{
  return send('d', pos);
}


namespace
{
uint8_t fromHex(const char h)
{
  if('0'<=h && h<'9')
    return h-'0';
  if('a'<=h && h<='f')
    return 10+(h-'a');
  throw Servo::ExceptionInvalidHex{static_cast<uint8_t>(h), "unexpected char"};
} // fromHex()

char toHex(const uint8_t num)
{
  if(num>0x0F)
    throw Servo::ExceptionInvalidHex{num, "number out of range"};
  const char lut[]="0123456789abcdef";
  return lut[num];
} // fromHex()

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
  return toHex(out);
} // computeChecksum()
} // unnamed namespace


Response Servo::send(char cmd, uint8_t pos)
{
  // TODO: fix this: 
  const uint8_t posNum[2]={ static_cast<uint8_t>((pos&0xF0)>>4), static_cast<uint8_t>((pos&0x0F)>>0) };
  // create command
  char buf[1+1+2+1+1+1];
  buf[0]=name_.getName();
  buf[1]=cmd;
  buf[2]=toHex(posNum[0]);
  buf[3]=toHex(posNum[1]);
  //buf[4]='?';      // if checksums are not needed ;)
  buf[4]=computeChecksum(buf[0], buf[1], buf[2], buf[3]);
  buf[5]='\n';
  buf[6]=0;
  // send it
  return dev_->run(buf);
}

} // namespace ServoCtrl
