#include <algorithm>
#include <cstring>
#include <cassert>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "ServoCtrl/CommDevice.hpp"

namespace ServoCtrl
{

CommDevice::CommDevice(const boost::filesystem::path &devPath):
  wr_{ new SerialPort{devPath, SerialPort::WRITE} },
  rd_{ new SerialPort{devPath, SerialPort::READ} },
  th_{rd_}
{
}

Response CommDevice::run(std::string cmd)
{
  const ServoName sn{cmd[0]};   // can be terminating zero, in case of empty string
  wr_->writeLine( std::move(cmd) );
  return th_.enqueue(sn);
}

} // namespace ServoCtrl
