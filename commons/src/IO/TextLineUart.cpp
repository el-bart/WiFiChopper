#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "IO/TextLineUart.hpp"
#include "Util/tabSize.hpp"
#include "Util/ErrStrm.hpp"


namespace IO
{

TextLineUart::CannotOpenDevice::CannotOpenDevice(const std::string& dev):
  Exception( (Util::ErrStrm{}<<"cannot open UART device: '"<<dev<<"'").str() )
{ }

TextLineUart::ErrorIO::ErrorIO(const std::string& dev, const char *details):
  Exception( (Util::ErrStrm{}<<"error while talking to the UART device '"<<dev<<"': "<<details<<": "<<strerror(errno)).str() )
{ }



TextLineUart::TextLineUart(const std::string& dev):
  dev_(dev),
  fd_( open( dev_.c_str(), 0 ) )
{
  if( fd_.get() == -1 )
    throw CannotOpenDevice(dev_);
}


size_t TextLineUart::sendSome(const Data& data, const size_t skip)
{
  const int ret = write( fd_.get(), data.data()+skip, data.size()-skip );
  if( ret == -1 )
    throw ErrorIO(dev_, "syscall write() failed");
  return ret;
}


size_t TextLineUart::readSome(Data& data, const double timeout)
{
  // try read some data
  uint8_t   buf[1024];
  const int ret = read( fd_.get(), buf, Util::tabSize(buf) );
  if( ret == -1 )
    throw ErrorIO(dev_, "syscall read() failed");
  // copy data to output buffer
  data.reserve( data.size()+ret );
  copy( buf, buf+ret, back_insert_iterator<Data>(data) );
  // return with the good news
  return ret;
}

}
