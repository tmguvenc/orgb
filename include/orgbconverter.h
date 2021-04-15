#ifndef ORGBCONVERTER_H
#define ORGBCONVERTER_H

#include <opencv2/core/mat.hpp>

class ORGBConverter
{
public:
    ORGBConverter();

    cv::Mat convert(const cv::Mat& srcImage);

private:
    cv::Mat transformMatrix;
    cv::Mat invTransformMatrix;
};

#endif // ORGBCONVERTER_H
