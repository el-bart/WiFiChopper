#include <algorithm>
#include <cstring>
#include <cassert>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "ServoCtrl/SerialPort.hpp"

namespace ServoCtrl
{

typedef std::unique_lock<std::mutex> Lock;

SerialPort::AutoDescriptor::AutoDescriptor(const int fd):
  fd_(fd)
{
}

SerialPort::AutoDescriptor::~AutoDescriptor(void)
{
  if(fd_!=-1)
    close(fd_);
}


namespace
{
int dirMode(const int dir)
{
  if( (dir&SerialPort::READ) && (dir&SerialPort::WRITE) )
    return O_RDWR;
  if(dir==SerialPort::READ)
    return O_RDONLY;
  if(dir==SerialPort::WRITE)
    return O_WRONLY;

  assert(!"unknown mode - update the code");
  return 0;
} // dirMode()


bool isEOL(const char c)
{
  return c=='\n' || c=='\r';
} // isEOL()
} // unnamed namespace


SerialPort::SerialPort(const boost::filesystem::path &devPath, const int dir):
  fd_( open(devPath.native().c_str(), dirMode(dir)|O_NOCTTY|O_NDELAY) )
{
  if(fd_.get()==-1)
    throw ExceptionDevice{devPath};
  configure();
  // this is a little trick - it appears that when RS-232 is started
  // it needs a little while to warm up. so we wait 10 20ms cycles
  // and cleanup any trashes that may have arrived.
  usleep(10*20*1000);
  if(dir&READ)
    discardContent();
}


void SerialPort::writeLine(std::string line)
{
  // if string does not end iwth new line, add it
  if( line.length()==0 || !isEOL( *line.rbegin() ) )
    line+="\n";
  // now send the data
  Lock lock{m_};
  writeData( std::move(line) );
  return;
}


std::string SerialPort::readLine(const unsigned int timeout)
{
  Lock lock{m_};
  return recvData(timeout);
}


void SerialPort::configure(void)
{
  // sets no other options (TODO: is this needed at all?)
  if( fcntl(fd_.get(), F_SETFL, 0)==-1 )
    throw ExceptionIO{"fcntl() failed"};

  // initial settings
  struct termios opts;
  memset(&opts, 0, sizeof(opts));

  // set baud rate
  const speed_t speed=B38400;
  cfsetispeed(&opts, speed);
  cfsetospeed(&opts, speed);
  // 8N1 (8 data bits, no parity, 1 stop bit)
  opts.c_cflag &= ~PARENB;
  opts.c_cflag &= ~CSTOPB;
  opts.c_cflag &= ~CSIZE;
  opts.c_cflag |=  CS8;
  // enable CRT/CTS hardware flow control
  opts.c_cflag |=  CRTSCTS;

  // apply these settings
  if( tcsetattr(fd_.get(), TCSANOW, &opts)!=0 )
    throw ExceptionIO{"tcsetattr(): setting new settings failed"};
}


void SerialPort::writeData(const std::string cmd)
{
  const int len=cmd.length();
  if( write(fd_.get(), cmd.c_str(), len)!=len )
    throw ExceptionIO{"write(): could not write command"};
}


std::string SerialPort::recvData(const unsigned int timeout)
{
  constexpr size_t  bufSize=32;     // TODO: magic value
  char              buf[bufSize];   // input buffer
  char             *ptr =buf;       // start from
  size_t            left=bufSize-1; // max more bytes to read


  while(true)   // note: will stop on timeout from select(), on data read
  {
    // wait for data or timeout
    fd_set rdfd;
    FD_ZERO(&rdfd);
    FD_SET(fd_.get(), &rdfd);
    struct timeval tout={0, timeout*1000};  // wait a while for the data
    const int ret=select(fd_.get()+1, &rdfd, nullptr, nullptr, &tout);
    if(ret<0)
      throw ExceptionIO{"select() failed for some reason"};
    if(ret==0)
      throw ExceptionTimeout{"response didn't arrived in expected time"};

    // read new data
    const int n=read(fd_.get(), ptr, 1);
    if(n<0)
      throw ExceptionIO{"read() failed for some reason"};

    // check for the end of line
    bool eol=false;
    std::for_each(ptr, ptr+n, [&eol](const char c){ if( isEOL(c) ) eol=true; });
    if(eol)
    {
      ptr[n]=0;
      assert( std::string{buf}.length()<bufSize );
      return std::string{buf};
    }

    // ok - something more to be read...
    ptr +=n;
    left-=n;

    // check for buffer end
    if(left==0)
      throw ExceptionIO{"buffer full of junk - discarding content"};
  } // while(true)

  assert(!"code never reaches here");
  return "?? code never reaches here ??";
}


void SerialPort::discardContent(void)
{
  long int dt=0;
  while(true)
  {
    struct timeval timeout={0, dt};
    fd_set rdfd;
    FD_ZERO(&rdfd);
    FD_SET(fd_.get(), &rdfd);
    const int ret=select(fd_.get()+1, &rdfd, nullptr, nullptr, &timeout);
    if(ret<=0)
      return;

    char tmp[128];
    if( read(fd_.get(), tmp, sizeof(tmp)-1)==-1 )
      throw ExceptionIO{"read(): unable to dispose of garbage in the buffers"};
    // if has some data already, wait a bit - maybe there's more to come?
    dt=6*1000;      // 6[ms] // TODO: magic value
  }
}

} // namespace ServoCtrl
