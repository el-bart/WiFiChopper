#ifndef INCLUDE_SERVOCTRL_SERVONAME_HPP_FILE
#define INCLUDE_SERVOCTRL_SERVONAME_HPP_FILE

/* public header */

#include "ServoCtrl/Exception.hpp"


namespace ServoCtrl
{

/** \brief representation of a servo name.
 */
class ServoName
{
public:
  /** \brief exception thrown on invalid name.
   */
  struct ExceptionInvalidServo: public Exception
  {
    /** \brief construct error message.
     *  \param name name that was not accepted as a servo name.
     */
    explicit ExceptionInvalidServo(const char name):
      Exception{ Strm{}<<"invalid servo name '"<<name<<"'" }
    {
    }
  }; // struct ExceptionInvalidServo


  /** \brief creates servo name object.
   *  \param name name to assign.
   */
  explicit ServoName(const char name):
    name_{ check(name) }
  {
  }

  /** \brief gets servo name.
   */
  char getName(void) const
  {
    return name_;
  }

  /** \brief gets first possible servo name.
   *  \return first name that can be used for servo.
   */
  static constexpr char first(void)
  {
    return 'a';
  }
  /** \brief gets last servo name.
   *  \return last name that can be used for servo.
   */
  static constexpr char last(void)
  {
    return 'q';
  }

private:
  char check(const char name)
  {
    if(name<first() || last()<name)
      throw ExceptionInvalidServo(name);
    return name;
  }

  char name_;
}; // class ServoName

} // namespace ServoCtrl

#endif
