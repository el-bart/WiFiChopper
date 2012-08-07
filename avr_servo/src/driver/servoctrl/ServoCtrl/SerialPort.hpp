#ifndef INCLUDE_SERVOCTRL_SERIALPORT_HPP_FILE
#define INCLUDE_SERVOCTRL_SERIALPORT_HPP_FILE

/* public header */

#include <mutex>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

#include "ServoCtrl/Exception.hpp"
#include "ServoCtrl/SharedPtrNotNull.hpp"

namespace ServoCtrl
{

/** \brief representation of physical serial port, used for communication.
 */
class SerialPort: private boost::noncopyable
{
public:
  /** \brief exception thrown on device-related issues.
   */
  struct ExceptionDevice: public Exception
  {
    /** \brief create error message.
     *  \param devPath path to device, that caused an error.
     */
    explicit ExceptionDevice(const boost::filesystem::path &devPath):
      Exception{ Strm{}<<"error while opening device: "<<devPath.native() }
    {
    }
  };

  /** \brief exception thrown on generic I/O errors.
   */
  struct ExceptionIO: public Exception
  {
    /** \brief creates error message.
     *  \param details error message details, to be added to the error report.
     */
    explicit ExceptionIO(std::string details):
      Exception{ Strm{}<<"I/O error: "<<std::move(details) }
    {
    }
  };

  /** \brief exception throw on timeout, while waiting for the responses.
   */
  struct ExceptionTimeout: public Exception
  {
    /** \brief creates error message.
     *  \param details error message details, to be attached to the report.
     */
    explicit ExceptionTimeout(std::string details):
      Exception{ Strm{}<<"timeout while reading data: "<<std::move(details) }
    {
    }
  };

  /** \brief communication direction.
   */
  enum Direction
  {
    READ =1,    ///< read form the device.
    WRITE=2     ///< write to the device.
  };


  /** \brief create servial port device representation.
   *  \param devPath path to the physical device.
   *  \param dir     direction of the communication channel (see 'Direction' enum).
   *  \note single device can be an I/O device as well.
   */
  SerialPort(const boost::filesystem::path &devPath, int dir);

  /** \brief sends given line.
   *  \param line lien to be sent.
   */
  void writeLine(std::string line);
  /** \brief reads line withing a given timeout.
   *  \param timeout [ms] to wait.
   *  \return line that has been read.
   */
  std::string readLine(unsigned int timeout);

private:
  void configure(void);
  void writeData(std::string cmd);
  std::string recvData(unsigned int timeout);
  void discardContent(void);

  class AutoDescriptor: private boost::noncopyable
  {
  public:
    explicit AutoDescriptor(const int fd);
    ~AutoDescriptor(void);
    int get(void)
    {
      return fd_;
    }

  private:
    int fd_;
  }; // struct AutoDescriptor

  std::mutex     m_;
  AutoDescriptor fd_;
}; // class SerialPort


typedef SharedPtrNotNull<SerialPort> SerialPortPtrNN;

} // namespace ServoCtrl

#endif
