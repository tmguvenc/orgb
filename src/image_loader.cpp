#include "image_loader.h"
#include <opencv2/highgui.hpp>

cv::Mat ImageLoader::readImage(const std::string& fileName) {
    return cv::imread(fileName);
}
