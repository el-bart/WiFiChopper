#include <iostream>
#include <opencv2/opencv.hpp>

#include "UsrInt/TextAdder.hpp"

using namespace std;

int main(void)
{
  cv::Mat                 img( cv::Size(400, 200), CV_8UC3, CV_RGB(0,255,0) );
  const UsrInt::TextAdder txt( 200, 20, CV_RGB(255,0,0), 1 );
  txt.addLine(img, 0, "ala ma kota");
  txt.addLine(img, 1, "druga linia");
  imshow( "text test", img );
  cv::waitKey( 3*1000 );
  return 0;
}
