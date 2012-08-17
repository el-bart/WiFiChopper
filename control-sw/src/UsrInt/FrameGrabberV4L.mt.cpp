#include <iostream>

#include "UsrInt/FrameGrabberV4L.hpp"
#include "Util/ClockTimer.hpp"

using namespace std;


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <video_device>" << endl;
    return 1;
  }

  UsrInt::FrameGrabberPtr fg( new UsrInt::FrameGrabberV4L(argv[1]) );
  const std::string winName = "image from grabber";
  cv::namedWindow( winName, CV_WINDOW_AUTOSIZE|CV_WINDOW_KEEPRATIO );
  const Util::ClockTimerRT clk;
  size_t i;
  for(i=0; i<1000; ++i)
  {
    cerr << ".";
    cv::Mat frame = fg->grab();
    // output
    imshow( winName, frame );
    if( cv::waitKey(10) >= 0 )
    {
      cout << endl;
      cout << "breaking on user request";
      break;
    }
  }
  cout << endl;
  cout << "~" << i/clk.elapsed() << " fps on average" << endl;
  cout << "all done! :)" << endl;

  return 0;
}
