#ifndef INCLUDE_IO_LINECOMM_HPP_FILE
#define INCLUDE_IO_LINECOMM_HPP_FILE

#include <string>
#include <memory>

#include "IO/TextLine.hpp"
#include "Net/Channel.hpp"
#include "Crypto/Algo.hpp"


namespace IO
{

class LineComm
{
public:
  struct Key: public Crypto::BinData
  {
    Key(void);
  };


  LineComm(Net::Channel chn, Key key);

  void sendLine(const std::string& line);
  std::string readLine(const double timeout);

private:
  std::unique_ptr<Crypto::Algo> crypt_;
  std::unique_ptr<IO::TextLine> comm_;
};

}

#endif
