#ifndef INCLUDE_IO_LINECOMMNETWORK_HPP_FILE
#define INCLUDE_IO_LINECOMMNETWORK_HPP_FILE

#include <string>
#include <memory>

#include "IO/LineComm.hpp"
#include "IO/TextLine.hpp"
#include "Net/Channel.hpp"
#include "Crypto/Algo.hpp"


namespace IO
{

class LineCommNetwork: public LineComm
{
public:
  struct Key: public Crypto::BinData
  {
    Key(void);
  };


  LineCommNetwork(Net::Channel chn, Key key);

  virtual void send(const std::string& line);
  virtual std::string read(const double timeout);

private:
  std::unique_ptr<Crypto::Algo> crypt_;
  std::unique_ptr<IO::TextLine> comm_;
};

}

#endif
