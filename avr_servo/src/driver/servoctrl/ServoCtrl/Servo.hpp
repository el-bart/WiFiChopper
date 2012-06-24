#ifndef INCLUDE_SERVOCTRL_SERVO_HPP_FILE
#define INCLUDE_SERVOCTRL_SERVO_HPP_FILE

/* public header */

#include "ServoCtrl/CommDevice.hpp"
#include "ServoCtrl/ServoName.hpp"
#include "ServoCtrl/Response.hpp"
#include "ServoCtrl/Exception.hpp"

namespace ServoCtrl
{

/** \brief representation of a single servo mechanism.
 */
class Servo
{
public:
  /** \brief exception throw on error in hex parsing.
   */
  struct ExceptionInvalidHex: public Exception
  {
    /** \brief constructs error message.
     *  \param byte    byte that could not be interpreted as a hex sign.
     *  \param details more exception details.
     */
    ExceptionInvalidHex(const uint8_t byte, std::string details):
      Exception{ Strm{}<<"invalid byte '"<<int{byte}<<"' ("<<byte<<") when converting to hex: "<<std::move(details) }
    {
    }
  }; // struct ExceptionInvalidHex

  /** \brief creates servo object.
   *  \param name name of the servo.
   *  \param dev  device to use for communication.
   */
  Servo(ServoName name, CommDevicePtrNN dev);

  /** \brief sets position to a given value.
   *  \param pos position to set.
   *  \return future-response object.
   */
  Response setPos(uint8_t pos);
  /** \brief sets minimal position to a given value.
   *  \param pos position to set.
   *  \return future-response object.
   */
  Response setMinPos(uint8_t pos);
  /** \brief sets maximal position to a given value.
   *  \param pos position to set.
   *  \return future-response object.
   */
  Response setMaxPos(uint8_t pos);
  /** \brief sets default position to a given value.
   *  \param pos position to set.
   *  \return future-response object.
   */
  Response setDefaultPos(uint8_t pos);
  /** \brief gets servo name.
   *  \return name of the servo this class controlls.
   */
  ServoName getName(void) const
  {
    return name_;
  }

private:
  Response send(char cmd, uint8_t pos);

  ServoName       name_;
  CommDevicePtrNN dev_;
};

} // namespace ServoCtrl

#endif
