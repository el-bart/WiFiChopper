#ifndef INCLUDE_USRINT_TESTADDER_HPP_FILE
#define INCLUDE_USRINT_TESTADDER_HPP_FILE

#include <string>
#include <opencv2/opencv.hpp>


namespace UsrInt
{

class TextAdder
{
public:
  TextAdder(size_t wSpace, size_t hSpace, CvScalar color, double scale = 1.0);

  void addLine(cv::Mat& img, unsigned int lineNo, const std::string& txt) const;

private:
  size_t getFontHeight(void) const;

  const size_t   wSpace_;
  const size_t   hSpace_;
  const double   scale_;
  const CvFont   font_;
  const CvScalar color_;
  const size_t   fontHeight_;
};

}

#endif
