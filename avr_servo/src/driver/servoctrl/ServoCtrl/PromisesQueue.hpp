#ifndef INCLUDE_SERVOCTRL_PROMISESQUEUE_HPP_FILE
#define INCLUDE_SERVOCTRL_PROMISESQUEUE_HPP_FILE

/* public header */

#include <deque>
#include <mutex>
#include <condition_variable>
#include <boost/noncopyable.hpp>

#include "ServoCtrl/TimerRT.hpp"
#include "ServoCtrl/Response.hpp"
#include "ServoCtrl/ServoName.hpp"
#include "ServoCtrl/Exception.hpp"


namespace ServoCtrl
{

/** \brief queue of response promises.
 */
class PromisesQueue: private boost::noncopyable
{
public:
  /** \brief queue's element.
   */
  struct Element
  {
    ServoName         s_;   ///< name of the servo, response should arrive for.
    TimerRT           t_;   ///< timer, counting since creation of this object.
    Response::Promise p_;   ///< promise object.
  }; // struct Element

  /** \brief adds element to the queue.
   *  \param s servo to wait for response.
   *  \return future object, for the response.
   */
  Response::Future push(ServoName s);
  /** \brief gets element to be processed.
   *  \return element to process.
   */
  Element pop(void);

private:
  typedef std::deque<Element> Promises;

  std::mutex              m_;
  std::condition_variable nonEmpty_;
  Promises                queue_;
}; // class PromisesQueue

} // namespace ServoCtrl

#endif
