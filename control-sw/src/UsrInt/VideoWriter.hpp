#ifndef INCLUDE_USRINT_VIDEOWRITER_HPP_FILE
#define INCLUDE_USRINT_VIDEOWRITER_HPP_FILE

#include <string>
#include <memory>
#include <opencv2/highgui/highgui.hpp>

#include "Util/ClockTimer.hpp"
#include "UsrInt/FrameGrabber.hpp"


namespace UsrInt
{

class VideoWriter
{
public:
  VideoWriter(std::string outFile, double fps);
  VideoWriter(std::string outFile, FrameGrabber& fg);
  // NOTE: explicit mv-c-tor is required since OpenCV's members have copy c-tors, by do not have
  //       mv-c-tors, thus making them disabled by default, thus blocking movability of this object.
  VideoWriter(VideoWriter&& other):
    outFile_( std::move(other.outFile_) ),
    fps_(other.fps_),
    size_(other.size_),
    vw_( std::move(other.vw_) ),
    fpsClk_(other.fpsClk_),
    frames_(other.frames_)
  { }

  void writeFrame(const cv::Mat& img);

  const std::string& fileName(void) const { return outFile_; }
  double fps(void) const { return fps_; }
  cv::Size size(void) const { return size_; }

private:
  void measureFps(FrameGrabber& fg);
  void measureFps(const cv::Mat& img);
  void initWriter(double fps, cv::Size size);
  void writeFrameImpl(const cv::Mat& img);
  void grabNoErr(FrameGrabber& fg);

  std::string                      outFile_;
  double                           fps_;
  cv::Size                         size_;
  std::unique_ptr<cv::VideoWriter> vw_;
  Util::ClockTimerRT               fpsClk_;
  size_t                           frames_;
};

}

#endif
