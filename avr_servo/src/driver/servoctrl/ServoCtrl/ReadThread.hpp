#ifndef INCLUDE_SERVOCTRL_READTHREAD_HPP_FILE
#define INCLUDE_SERVOCTRL_READTHREAD_HPP_FILE

/* public header */

#include <thread>

#include "ServoCtrl/PromisesQueue.hpp"
#include "ServoCtrl/SerialPort.hpp"

namespace ServoCtrl
{

/** \brief thread reading data from given read-device.
 */
class ReadThread
{
public:
  /** \brief creates reader thread object.
   *  \param rd device to read from.
   */
  explicit ReadThread(SerialPortPtrNN rd);
  /** \brief stops thread and frees resources.
   */
  ~ReadThread(void);

  /** \brief adds new request to the processing queue.
   *  \param s name of the servo responses expected for.
   *  \return response object (future).
   */
  Response enqueue(ServoName s);

private:
  void threadBody(void);

  bool            quit_;
  SerialPortPtrNN rd_;
  PromisesQueue   q_;
  std::thread     th_;
}; // class ReadThread

} // namespace ServoCtrl

#endif
