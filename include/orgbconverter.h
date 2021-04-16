#ifndef ORGBCONVERTER_H
#define ORGBCONVERTER_H

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

class ORGBConverter
{
public:
    ORGBConverter(const cv::Mat& srcImage);

    cv::Mat getOrgbImage(const cv::Vec3d& scaleFactor, const cv::Vec3d& shiftFactor) const;

private:
    cv::Mat convert(const cv::Mat& srcImage);
    cv::Mat orgbImage;
};

#endif // ORGBCONVERTER_H
