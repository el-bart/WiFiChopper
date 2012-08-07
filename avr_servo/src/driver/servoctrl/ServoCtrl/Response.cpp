#include <stdexcept>
#include <cassert>

#include "ServoCtrl/Response.hpp"


namespace ServoCtrl
{

Response::ExceptionProtocolError::ExceptionProtocolError(std::string message):
  Exception{ Strm{}<<"protocol reported an error: "<<std::move(message) }
{
}


Response::Response(Future fut):
  isSet_{false},
  fut_{ std::move(fut) }
{
}


bool Response::ok(void)
{
  return !error();
}


bool Response::error(void)
{
  process();
  return static_cast<bool>(ex_);
}


std::exception_ptr Response::getError(void)
{
  process();
  return ex_;
}


std::string Response::getErrorMessage(void)
{
  // no error at all?
  if( !error() )
    return "no error";
  assert(ex_ && "no exception saved when an error is reported");

  // any other error
  try
  {
    std::rethrow_exception(ex_);
    assert("!this code is never reached");
  }
  catch(const std::exception &ex)
  {
    return ex.what();
  }
  catch(...)
  {
    assert(!"INTERNAL ERROR: unknwon exception type");
    return "INTERNAL ERROR: unknwon exception type";
  }
  assert(!"code never reaches here");
  return "code never reaches here";
}


void Response::process(void)
{
  // response can be read just once
  if(isSet_)
    return;
  isSet_=true;  // NOTE: if an error is reported, it will be saved as the final response
  assert(!ex_ && "exception already set");

  try
  {
    // read the response
    const std::string resp=fut_.get();
    // in case of protocol error, prepare proper exception
    if( resp.find("ERR")!=std::string::npos )
      ex_=std::copy_exception( ExceptionProtocolError{resp} );
  }
  catch(...)
  {
    ex_=std::current_exception();
  }
}

} // namespace ServoCtrl
