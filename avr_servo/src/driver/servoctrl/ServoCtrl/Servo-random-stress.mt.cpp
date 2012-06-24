#include <iostream>
#include <random>
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

  // prepare random generators
  uniform_int_distribution<> uniSrvNo( 0, ServoName::last()-ServoName::first() );
  uniform_int_distribution<> uniPos(50, 200);
  random_device              rndDev;
  knuth_b                    rndEng{rndDev()};
  auto                       rndSrvNo=[&]{ return uniSrvNo(rndEng); };
  auto                       rndPos  =[&]{ return uniPos(rndEng); };

  while(true)
  {
    usleep(delay*1000);
    const uint8_t srvNo=rndSrvNo();
    const uint8_t pos  =rndPos();
    cout<<"sending pos "<<int{pos}<<" to servo "<<static_cast<char>(ServoName::first()+srvNo)<<"..."<<endl;
    try
    {
      Response r=servos.at(srvNo).setPos(pos);
      if( !fast && r.error() )
        throw std::runtime_error("error while setting device");
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
  } // while(true)

  return 0;
}
