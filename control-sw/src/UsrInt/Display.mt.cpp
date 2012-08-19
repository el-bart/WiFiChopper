#include <iomanip>
#include <iostream>
#include <algorithm>

#include "UsrInt/FrameGrabberV4L.hpp"
#include "UsrInt/Display.hpp"

using namespace std;


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <video_device>" << endl;
    return 1;
  }

  UsrInt::FrameGrabberPtr fg( new UsrInt::FrameGrabberV4L(argv[1], 800, 600) );
  UsrInt::Display         disp( std::move(fg) );

  cout << "grabbing frames - press any key to stop" << endl;
  size_t i;
  for(i=0; true; ++i)
  {
    try
    {
      const UsrInt::Display::Info info = { 11.12, {1,2,3} };
      disp.update(info);
      cerr << ".";
      if( cv::waitKey(10) >= 0 )
      {
        cout << endl;
        cout << "breaking on user request";
        break;
      }
    }
    catch(const std::exception& ex)
    {
      cerr << 'X' << endl;
      cerr << ex.what() << endl;
    }
  }
  cout << endl;
  cout << "all done! :)" << endl;

  return 0;
}
