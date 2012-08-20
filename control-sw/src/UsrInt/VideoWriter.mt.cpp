#include <iomanip>
#include <iostream>

#include "UsrInt/FrameGrabberV4L.hpp"
#include "UsrInt/VideoWriter.hpp"

using namespace std;


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <video_device>" << endl;
    return 1;
  }

  cout << "initializing device..." << endl;
  UsrInt::FrameGrabberPtr fg( new UsrInt::FrameGrabberV4L(argv[1], 800, 600) );

  //UsrInt::VideoWriter vw("some_test_video.avi", 15);
  UsrInt::VideoWriter vw("some_test_video.avi", *fg);
  cout << vw.size().width << "x" << vw.size().height << " @ " << vw.fps() << " fps; writing to '" << vw.fileName() << "'" << endl;

  cout << "grabbing frames - press 'q' key to stop" << endl;
  const Util::ClockTimerRT clkFps;
  size_t                   i;
  for(i=0; true; ++i)
  {
    try
    {
      const cv::Mat frame = fg->grab();
      cerr << ".";
      // output
      imshow( "preview", frame );
      if( cv::waitKey(10) == 'q' )
      {
        cout << endl;
        cout << "breaking on user request";
        break;
      }
      // write
      vw.writeFrame(frame);
    }
    catch(const std::exception& ex)
    {
      cerr << 'X' << endl;
      cerr << ex.what() << endl;
    }
  }
  const double fpsT = clkFps.elapsed();
  cout << endl;
  cout << "processed " << i << " frames in " << fpsT << " second(s) (" << i/clkFps.elapsed() << " fps on average)" << endl;
  cout << "all done! :)" << endl;

  return 0;
}
