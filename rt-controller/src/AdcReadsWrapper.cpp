#include "config.hpp"

#include "AdcReadsWrapper.hpp"


AdcReadsWrapper::AdcReadsWrapper(void):
  step_(0)
{
  // ensure at least one data is ready
  do { step(); }
  while(step_!=0);  // loop until cycle restarts
}

void AdcReadsWrapper::step(void)
{
  switch(step_)
  {
    case 0: // init read OX
         adc_.startReadOX();
         ++step_;
         break;

    case 1: // wait until OX is ready, read it and start OY
         if( adc_.isReadReady() )
         {
           now_.ox_=adc_.getRead();
           adc_.startReadOY();
           ++step_;
         }
         break;

    case 2: // wait until OY is ready, read it and start OZ
         if( adc_.isReadReady() )
         {
           now_.oy_=adc_.getRead();
           adc_.startReadOZ();
           ++step_;
         }
         break;

    case 3: // wait until OZ is ready, read it and start Vin
         if( adc_.isReadReady() )
         {
           now_.oz_=adc_.getRead();
           adc_.startReadVin();
           ++step_;
         }
         break;

    case 4: // wait until Vin is ready, read it, save new last_ value and restart
         if( adc_.isReadReady() )
         {
           now_.vin_=adc_.getRead();
           step_=0;
           // average last 2 reads
           last_.ox_  = (static_cast<uint16_t>(last_.ox_)  + now_.ox_ ) / 2;
           last_.oy_  = (static_cast<uint16_t>(last_.oy_)  + now_.oy_ ) / 2;
           last_.oz_  = (static_cast<uint16_t>(last_.oz_)  + now_.oz_ ) / 2;
           last_.vin_ = (static_cast<uint16_t>(last_.vin_) + now_.vin_) / 2;
         }
         break;

    default: // wtf?!
         step_=0;
         break;
  } // switch(step)
}
