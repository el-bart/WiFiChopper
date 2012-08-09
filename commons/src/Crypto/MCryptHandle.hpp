#ifndef INCLUDE_CRYPTO_MCRYPTHANDLE_HPP_FILE
#define INCLUDE_CRYPTO_MCRYPTHANDLE_HPP_FILE

#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <mcrypt.h>

namespace Crypto
{

class MCryptHandle
{
public:
  explicit MCryptHandle(const std::string& algo, const std::string& mode);
  ~MCryptHandle(void);

  // noncopyable
  MCryptHandle(const MCryptHandle&) = delete;
  MCryptHandle& operator=(const MCryptHandle&) = delete;

  // movable
  MCryptHandle(MCryptHandle&& other):
    algo_( std::move(other.algo_) ),
    mode_( std::move(other.mode_) ),
    td_(other.td_)
  {
    other.td_ = MCRYPT_FAILED;
  }
  MCryptHandle& operator=(MCryptHandle&& other)
  {
    MCryptHandle tmp( std::move(other) );
    swap(tmp);
    return *this;
  }

  MCRYPT get(void) { return td_; }

  const char* algo(void) const { return algo_.get(); }
  const char* mode(void) const { return mode_.get(); }

  void swap(MCryptHandle& other)
  {
    std::swap(algo_, other.algo_);
    std::swap(mode_, other.mode_);
    std::swap(td_,   other.td_  );
  }

private:
  typedef std::unique_ptr<char[]> CString;

  static CString copyStr(const std::string &str);

  CString algo_;
  CString mode_;
  MCRYPT  td_;
};


inline void swap(MCryptHandle& first, MCryptHandle& second)
{
  first.swap(second);
}

}

#endif
