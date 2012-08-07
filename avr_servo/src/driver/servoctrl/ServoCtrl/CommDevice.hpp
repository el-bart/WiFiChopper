#ifndef INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE
#define INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

#include "ServoCtrl/SharedPtrNotNull.hpp"
#include "ServoCtrl/SerialPort.hpp"
#include "ServoCtrl/ReadThread.hpp"
#include "ServoCtrl/ServoName.hpp"

namespace ServoCtrl
{

/** \brief high-level representation of communication device.
 */
class CommDevice: private boost::noncopyable
{
public:
  /** \brief create device instance.
   *  \param devPath path to the actual device to use for communication.
   */
  explicit CommDevice(const boost::filesystem::path &devPath);

  /** \brief sends given command to the hardware driver.
   *  \param cmd command to be send.
   *  \return future-response object.
   */
  Response run(std::string cmd);

private:
  SerialPortPtrNN wr_;
  SerialPortPtrNN rd_;
  ReadThread      th_;
}; // class CommDevice


/** \brief communication device pointer. */
typedef SharedPtrNotNull<CommDevice> CommDevicePtrNN;

} // namespace ServoCtrl

#endif
