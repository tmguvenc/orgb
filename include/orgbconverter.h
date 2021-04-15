#ifndef ORGBCONVERTER_H
#define ORGBCONVERTER_H

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

struct LCC;

class ORGBConverter
{
public:
    ORGBConverter(const cv::Mat& srcImage);

    cv::Mat getOrgbImage(const LCC& scaleFactor, const LCC& shiftFactor) const;

private:
    cv::Mat convert(const cv::Mat& srcImage);
    cv::Mat orgbImage;
};

#endif // ORGBCONVERTER_H
