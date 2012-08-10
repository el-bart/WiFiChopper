#include <iostream>

#include "IO/LineCommUart.hpp"

using namespace std;

int main(int argc, char **argv)
{
  IO::LineCommPtr lcp( new IO::LineCommUart("/dev/ttyUSB1") );
  lcp->send("hello");
  cout << lcp->read(1.0) << endl;
  return 0;
}
