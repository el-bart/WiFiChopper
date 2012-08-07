#include <cassert>

#include "ServoCtrl/ReadThread.hpp"


namespace ServoCtrl
{

ReadThread::ReadThread(SerialPortPtrNN rd):
  quit_{false},
  rd_{rd},
  th_{ [&]{ threadBody(); } }
{
}


ReadThread::~ReadThread(void)
{
  quit_=true;
  try
  {
    // add something to queue
    q_.push( ServoName{ ServoName::first() } );
    // wait for thread to end
    if( th_.joinable() )
      th_.join();
  }
  catch(...)
  {
    // nothing more can be done...
    th_.detach();
  }
}


Response ReadThread::enqueue(ServoName s)
{
  return Response{ q_.push(s) };
}


void ReadThread::threadBody(void)
{
  while(!quit_)
  {
    try
    {
      PromisesQueue::Element e=q_.pop();
      try
      {
        // maximal timeout must be truncated, when some time already passed since sending
        const unsigned int maxTimeout=4*20;                 // TODO: hardcoded value
        const unsigned int elapsed   =e.t_.elapsed()*1000;  // [s]->[ms]
        const unsigned int timeout   =(maxTimeout<=elapsed)?1:(maxTimeout-elapsed);
        assert(timeout<=maxTimeout);
        std::string ret=rd_->readLine(timeout);
        // fulfill promise
        e.p_.set_value( std::move(ret) );
      }
      catch(...)
      {
        e.p_.set_exception( std::current_exception() );
      }
    }
    catch(const std::exception &)
    {
      // TODO: what to do with this error?
    }
  } // while(!quit_)
}

} // namespace ServoCtrl
