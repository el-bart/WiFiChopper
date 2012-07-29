#include "Tokenizer.hpp"

const char *Tokenizer::getNextToken(void)
{
  // no more?
  if(tokens_==0)
    return nullptr;

  // save pointer to be returned
  const char *tmp=buf_;

  // move to the next token
  --tokens_;
  while(*buf_!=0)
    ++buf_;
  // skip token separator
  ++buf_;
  // if there are more tokens, skip tokens separators (can be multiple of them!
  if(tokens_>0)
    while(*buf_==0)
      ++buf_;

  // return pointer
  return tmp;
}


namespace
{
inline bool isEOS(const char c)
{
  return c==0;
}
inline bool isSeparator(const char c)
{
  return c==' ' || c=='\t';
}
}


uint8_t Tokenizer::tokenize()
{
  if(buf_==nullptr)
    return 0;

  // sanitization
  while( isSeparator(*buf_) ) ++buf_;       // skip leading separators
  if( isEOS(*buf_) )                        // nothing inside?
    return 0;

  // process until the end of the string
  char*   s    =buf_;
  uint8_t count=1;
  while(true)
  {
    // end of string means end of processing
    if( isEOS(*s) )
      return count;

    // field separators
    if( isSeparator(*s) )
    {
      // skip all of the separators
      while( isSeparator(*s) )
      {
        *s=0;       // mark this as the end of the token
        ++s;
      }
      // mark next token, only if not the end of the sequence
      if( !isEOS(*s) )
        ++count;
    }

    // proceed to the next character
    ++s;
  }

  return count;
}
