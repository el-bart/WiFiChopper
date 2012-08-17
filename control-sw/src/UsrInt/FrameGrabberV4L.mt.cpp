#include <iomanip>
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

  cout << setprecision(3);

  cout << "initializing device..." << endl;
  const Util::ClockTimerRT clkInit;
  UsrInt::FrameGrabberPtr fg( new UsrInt::FrameGrabberV4L(argv[1], 800, 600) );
  const std::string winName = "image from grabber";
  cv::namedWindow( winName, CV_WINDOW_AUTOSIZE|CV_WINDOW_KEEPRATIO );
  cout << "device initialized in " << clkInit.elapsed() << " second(s)" << endl;

  // check what is the grabbing resolution
  {
    const cv::Size size = fg->grab().size();
    cout << "grabbing resolution set to " << size.width << "x" << size.height << endl;
  }

  cout << "grabbing frames - press any key to stop" << endl;
  const Util::ClockTimerRT clkFps;
  size_t                   i;
  for(i=0; true; ++i)
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
  const double fpsT = clkFps.elapsed();
  cout << endl;
  cout << "processed " << i << " frames in " << fpsT << " second(s) (" << i/clkFps.elapsed() << " fps on average)" << endl;
  cout << "all done! :)" << endl;

  return 0;
}
