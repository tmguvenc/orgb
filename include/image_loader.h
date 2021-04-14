#ifndef IMAGE_LOADE_H
#define IMAGE_LOADE_H

#include <opencv2/core/mat.hpp>
#include <string>

class ImageLoader
{
public:
    static cv::Mat readImage(const std::string& fileName);
};

#endif // IMAGE_LOADE_H
