#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "ServoCtrl/Servo.hpp"

using namespace std;
using namespace ServoCtrl;


int main(int argc, char **argv)
{
  if(argc!=1+4)
  {
    cerr<<argv[0]<<" <dev> <delay-ms> <fast[01]> <exit-on-error[01]>"<<endl;
    return 1;
  }
  const unsigned int delay=boost::lexical_cast<unsigned int>(argv[2]);
  const bool         fast =boost::lexical_cast<bool>(argv[3]);
  const bool         eoe  =boost::lexical_cast<bool>(argv[4]);

  // create communication device
  CommDevicePtrNN dev{ new CommDevice{argv[1]} };
  // create collection of servos
  std::vector<Servo> servos;
  servos.reserve( ServoName::last()-ServoName::first() + 1 );
  for(auto sn=ServoName::first(); sn!=ServoName::last()+1; ++sn)
    servos.push_back( Servo{ ServoName{sn}, dev } );

  uint8_t pos=256/2;
  int     dp =+1;
  while(true)
  {
    // give servos a while to set on positions
    usleep(delay*1000);

    // prepare memory for output elements
    std::vector<Response> rets;
    rets.reserve( servos.size() );

    // select proper value to be set
    pos+=dp;
    if(pos>200)
      dp=-1;
    if(pos<50)
      dp=+1;

    // set values to servos
    for(auto it=servos.begin(); it!=servos.end(); ++it)
    {
      cout<<"sending servo "<<it->getName().getName()<<" to pos "<<int{pos}<<endl;
      rets.push_back( it->setPos(pos) );
    } // for(servo)

    // check the responses
    if(!fast)
    {
      for(auto it=rets.begin(); it!=rets.end(); ++it)
      {
        if( it->ok() )
          continue;
        // error...
        const char sn=static_cast<char>( ServoName::first() + (it-rets.begin()) );
        cerr<<argv[0]<<": @"<<int{pos}<<": cannot set required position for servo "<<sn<<": "<<it->getErrorMessage()<<endl;
        if(eoe)
        {
          cerr<<argv[0]<<": exiting of first error, as requested..."<<endl;
          return 2;
        }
      } // for(responses)
    } // if(!fast)

    // split calls
    cout<<"-------------"<<endl;
  } // while(true)

  return 0;
}
