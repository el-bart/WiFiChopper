#ifndef INCLUDE_SERVOCTRL_EXCEPTION_HPP_FILE
#define INCLUDE_SERVOCTRL_EXCEPTION_HPP_FILE

/* public header */

#include <string>
#include <sstream>
#include <stdexcept>

namespace ServoCtrl
{

/** \brief basic exception class.
 */
struct Exception: public std::runtime_error
{
  /** \brief builds exception message.
   *  \param message to be represented.
   */
  explicit Exception(std::string message):
    std::runtime_error{ std::move(message) }
  {
  }

protected:
  /** \brief helper stream object, used for creating messages.
   */
  struct Strm
  {
  public:
    /** \brief adds data to the output stream.
     *  \param t data to be added.
     *  \return reference to self.
     */
    template<typename T>
    Strm &operator<<(const T &t)
    {
      ss_<<t;
      return *this;
    }
    /** \brief extracts final message.
     *  \return final message.
     */
    operator std::string(void) const
    {
      return ss_.str();
    }

  private:
    std::stringstream ss_;
  }; // struct Strm
}; // struct Exception

} // namespace ServoCtrl

#endif
