#ifndef INCLUDE_TOKENIZER_HPP_FILE
#define INCLUDE_TOKENIZER_HPP_FILE

#include <inttypes.h>

class Tokenizer
{
public:
  explicit Tokenizer(char *buf):
    buf_(buf),
    tokens_( tokenize() )
  {
  }

  const char *getNextToken(void);

private:
  uint8_t tokenize();

  char    *buf_;
  uint8_t  tokens_;
};

#endif
