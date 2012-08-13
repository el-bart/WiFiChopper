#include <iterator>
#include <algorithm>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>


#include "IO/TextLineUart.hpp"
#include "Util/tabSize.hpp"
#include "Util/timedSelect.hpp"
#include "Util/ErrStrm.hpp"

using namespace std;


namespace IO
{

TextLineUart::CannotOpenDevice::CannotOpenDevice(const std::string& dev):
  Exception( (Util::ErrStrm{}<<"cannot open UART device: '"<<dev<<"'").str() )
{ }

TextLineUart::ErrorIO::ErrorIO(const std::string& dev, const char *details):
  Exception( (Util::ErrStrm{}<<"error while talking to the UART device '"<<dev<<"': "<<details<<": "<<strerror(errno)).str() )
{ }

TextLineUart::Timeout::Timeout(const std::string& dev):
  Exception( (Util::ErrStrm{}<<"timeout occured while reading from UART device: '"<<dev<<"'").str() )
{ }



TextLineUart::TextLineUart(const std::string& dev):
  dev_(dev),
  fd_( open( dev_.c_str(), O_RDWR|O_NOCTTY|O_NDELAY ) )
{
  // check if open worked
  if( fd_.get() < 0 )
    throw CannotOpenDevice(dev_);
  // set all required parameters
  configure();
}


size_t TextLineUart::sendSome(const Data& data, const size_t skip)
{
  const int ret = ::write( fd_.get(), data.data()+skip, data.size()-skip );
  if( ret == -1 )
    throw ErrorIO(dev_, "syscall write() failed");
  return ret;
}


size_t TextLineUart::readSome(Data& data, const double timeout)
{
  // try reading data, if present
  if( !Util::timedSelect( fd_.get(), timeout ) )
    throw Timeout(dev_);
  // try read some data
  uint8_t   buf[1024];
  const int ret = ::read( fd_.get(), buf, Util::tabSize(buf) );
  if( ret == -1 )
    throw ErrorIO(dev_, "syscall read() failed");
  // copy data to output buffer
  data.reserve( data.size()+ret );
  copy( buf, buf+ret, back_insert_iterator<Data>(data) );
  // return with the good news
  return ret;
}


void TextLineUart::configure(void)
{
  // reest flags
  fcntl( fd_.get() , F_SETFL, 0 );

  // reset content of the configuration and readcurrent one
  struct termios port_settings;
  memset(&port_settings, 0, sizeof(port_settings));

  // mode: set no parity, stop bits, data bits
  port_settings.c_iflag = IGNBRK;               // input flags
  port_settings.c_oflag = 0;                    // output flags
  port_settings.c_cflag = CREAD | CS8 | CLOCAL; // control flags
  port_settings.c_lflag = 0;                    // noncannonical mode, no echo...

  // speed setup
  constexpr auto spd = B38400;
  cfsetispeed(&port_settings, spd);             // in
  cfsetospeed(&port_settings, spd);             // out

  tcflush(fd_.get(), TCIOFLUSH);                // flush data not yet sent/received

  // apply setting to the port
  if( tcsetattr(fd_.get(), TCSANOW, &port_settings) == -1 ) // apply the settings to the port
    throw ErrorIO(dev_, "syscall tcsetattr() failed");
}

}
