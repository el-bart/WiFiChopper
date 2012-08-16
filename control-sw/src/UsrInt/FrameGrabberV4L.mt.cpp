#include <iostream>

#include "UsrInt/FrameGrabberV4L.hpp"

using namespace std;


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <video_device>" << endl;
    return 1;
  }

  UsrInt::FrameGrabberPtr fg( new UsrInt::FrameGrabberV4L(argv[1]) );
  cv::Mat frame = fg->grab();
  // output
  imshow("image from grabber", frame);
  cv::waitKey(3*1000);  // 3[s]

  return 0;
}
