#include <cstdint>
// buggy ffmpeg stuff - cstdint needs to be include before this one and extern "C" mark is needed for it to work...
extern "C" {
#include <libavutil/log.h>
}

#include "UsrInt/VideoWriter.hpp"


namespace UsrInt
{

namespace
{
constexpr size_t g_maxFrames = 30;
} // unnamed namespace


VideoWriter::VideoWriter(std::string outFile, const double fps):
  outFile_( std::move(outFile) ),
  fps_(fps),
  frames_(0)
{
  // NOTE: vm_ is left nullptr, since FPS will be auto-determined
}


VideoWriter::VideoWriter(std::string outFile, FrameGrabber& fg):
  outFile_( std::move(outFile) ),
  fps_(0),
  frames_(0)
{
  // measure fps and init writer
  measureFps(fg);
  initWriter(fps_, size_);
}


void VideoWriter::writeFrame(const cv::Mat& img)
{
  // if writer is not ready we still are in FPS measure mode
  if( vw_.get() == nullptr )
    measureFps(img);
  else
    writeFrameImpl(img);
}


void VideoWriter::measureFps(FrameGrabber& fg)
{
  fpsClk_.restart();
  for(size_t i=0; i<g_maxFrames; ++i)
    grabNoErr(fg);
  fps_ = frames_ / fpsClk_.elapsed();
}


void VideoWriter::measureFps(const cv::Mat& img)
{
  assert( vw_.get() == nullptr );
  if( frames_ == 0 )
    fpsClk_.restart();
  ++frames_;
  size_ = img.size();
  if( frames_ >= g_maxFrames )
    initWriter( frames_ / fpsClk_.elapsed(), size_ );
}


void VideoWriter::initWriter(const double fps, const cv::Size size)
{
  // disable FFMpeg's log messages messing on the screen
  av_log_set_level(AV_LOG_QUIET);

  // startup
  fps_ = fps;
  //const auto codec = CV_FOURCC('M','J','P','G');
  const auto codec = CV_FOURCC('D','I','V','X');
  vw_.reset( new cv::VideoWriter( outFile_, codec, fps_, size, true ) );
}


void VideoWriter::writeFrameImpl(const cv::Mat& img)
{
  assert( vw_.get() != nullptr );
  vw_->write(img);
}


void VideoWriter::grabNoErr(FrameGrabber& fg)
{
  try
  {
    const cv::Mat img = fg.grab();
    // if grabbing succeeded, save info
    ++frames_;
    size_ = img.size();
  }
  catch(...)
  {
    // errors are ignored at this point
  }
}

}
