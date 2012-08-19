#include "UsrInt/TextAdder.hpp"


namespace UsrInt
{

TextAdder::TextAdder(const size_t wSpace, const size_t hSpace, const CvScalar color, const double scale):
  wSpace_(wSpace),
  hSpace_(hSpace),
  scale_(scale),
  font_( cvFont(scale_) ),
  color_(color),
  fontHeight_( getFontHeight() )
{ }


void TextAdder::addLine(cv::Mat& img, const unsigned int lineNo, const std::string& txt) const
{
  IplImage        tmp = img;
  const size_t    w   = img.size().width - wSpace_;
  const size_t    h   = hSpace_ + lineNo*((fontHeight_*150)/100);
  const cv::Point p(w, h);
  cvPutText( &tmp, txt.c_str(), p, &font_, color_ );
}


size_t TextAdder::getFontHeight(void) const
{
  CvSize size;
  cvGetTextSize("ABCDEFGHIJKLMNOPQRSTUWVXYZ" "abcdefghijklmnopqrstuwvxyz" "0123456789", &font_, &size, nullptr);
  return size.height;
}

}
