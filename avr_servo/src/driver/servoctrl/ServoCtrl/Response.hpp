#ifndef INCLUDE_SERVOCTRL_RESPONSE_HPP_FILE
#define INCLUDE_SERVOCTRL_RESPONSE_HPP_FILE

/* public header */

#include <string>
#include <future>
#include <exception>

#include "ServoCtrl/Exception.hpp"


namespace ServoCtrl
{

/** \brief response object, representing driver's response on given command.
 */
class Response
{
public:
  /** \brief exception throw on generic protocol error.
   */
  struct ExceptionProtocolError: public Exception
  {
    /** \brief create error message.
     *  \param message details to be saved.
     */
    explicit ExceptionProtocolError(std::string message);
  };

  /** \brief future object - short name. */
  typedef std::future<std::string>  Future;
  /** \brief promise object - short name. */
  typedef std::promise<std::string> Promise;

  /** \brief creates object from a given future.
   *  \param fut future to associate this object with.
   */
  explicit Response(Future fut);

  /** \brief checks if command succeeded.
   *  \return true if command was confirmed, false otherwise.
   */
  bool ok(void);
  /** \brief checks if command failed.
   *  \return true, if command returned an error, false otherwise.
   */
  bool error(void);

  /** \brief gets exception pointer, if risen as a response.
   *  \return pointer to exception, if it was risen, nullptr otherwise.
   */
  std::exception_ptr getError(void);

  /** \brief returns error message, when exception was risen.
   *  \return message with error details.
   *
   *  when no excepiton was risien or protocol error occured, proper message is
   *  used as an error message.
   */
  std::string getErrorMessage(void);

private:
  void process(void);

  bool               isSet_;
  Future             fut_;
  std::exception_ptr ex_;
}; // class Response

} // namespace ServoCtrl

#endif
