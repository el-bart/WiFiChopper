#include <iostream>
#include <boost/lexical_cast.hpp>

#include "ServoCtrl/Servo.hpp"

using namespace std;
using namespace ServoCtrl;

int main(int argc, char **argv)
{
  if(argc!=1+5)
  {
    cerr<<argv[0]<<" <dev> <servo> <delay-ms> <fast[01]> <exit-on-error[01]>"<<endl;
    return 1;
  }
  const unsigned int delay=boost::lexical_cast<unsigned int>(argv[3]);
  const bool         fast =boost::lexical_cast<bool>(argv[4]);
  const bool         eoe  =boost::lexical_cast<bool>(argv[5]);

  CommDevicePtrNN dev{ new CommDevice{argv[1]} };
  Servo           servo{ ServoName{argv[2][0]}, dev };

  uint8_t pos=50;
  uint8_t dp =1;
  while(true)
  {
    usleep(delay*1000);
    cout<<"sending pos "<<int{pos}<<" to servo "<<argv[2][0]<<"..."<<endl;
    try
    {
      Response r=servo.setPos(pos);
      if( !fast && r.error() )
        throw std::runtime_error("error while setting position");
    }
    catch(const std::exception &ex)
    {
      cerr<<argv[0]<<": @"<<int{pos}<<" ("<<typeid(ex).name()<<"): "<<ex.what()<<endl;
      if(eoe)
      {
        cerr<<argv[0]<<": exiting of first error, as requested..."<<endl;
        return 2;
      }
    }
    pos+=dp;
    if(pos>200)
      dp=-1;
    if(pos<50)
      dp=+1;
  } // while(true)

  return 0;
}
