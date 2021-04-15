#ifndef ORGBCONVERTER_H
#define ORGBCONVERTER_H

#include <opencv2/core/mat.hpp>

class ORGBConverter
{
public:
    ORGBConverter(const cv::Mat& srcImage);

private:
    cv::Mat convert(const cv::Mat& srcImage);
    cv::Mat orgbImage;
};

#endif // ORGBCONVERTER_H
