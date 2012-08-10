#ifndef INCLUDE_IO_LINECOMM_HPP_FILE
#define INCLUDE_IO_LINECOMM_HPP_FILE

#include <string>
#include <memory>


namespace IO
{

class LineComm
{
public:
  LineComm(void) { }
  virtual ~LineComm(void) { }

  // noncopyable:
  LineComm(const LineComm&) = delete;
  LineComm& operator=(const LineComm&) = delete;
  // nonmovable:
  LineComm(LineComm&&) = delete;
  LineComm& operator=(LineComm&&) = delete;

  virtual void send(const std::string& line) = 0;
  virtual std::string read(const double timeout) = 0;
};


typedef std::unique_ptr<LineComm> LineCommPtr;

}

#endif
