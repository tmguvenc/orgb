#include "orgbconverter.h"
#include "angletransformer.h"
#include "lcc.h"
#include <tuple>
#include <algorithm>

static constexpr double TransformMat[3][3] = {
    { 0.2990,  0.5870,  0.1140 },
    { 0.5000,  0.5000, -1.0000 },
    { 0.8660, -0.8660,  0.0000 }
};

static constexpr double InvTransformMat[3][3] = {
    { 1.0000,  0.1140,  0.7436 },
    { 1.0000,  0.1140, -0.4111 },
    { 1.0000, -0.8660,  0.1663 }
};

ORGBConverter::ORGBConverter(const cv::Mat& srcImage) {
    orgbImage = convert(srcImage);
}

LCC createLCC(const cv::Vec3d& scaledPixel) {
    double lcc[3];

    for (auto row = 0; row < 3; ++row) {
        lcc[row] = 0;
        for (auto col = 0; col < 3; ++col) {
            lcc[row] += TransformMat[row][col] * scaledPixel[col];
        }
    }

    return {lcc[0], lcc[1], lcc[2]};
}

cv::Mat ORGBConverter::convert(const cv::Mat& srcImage) {
    if(srcImage.type() != CV_8UC3) {
        throw std::runtime_error("Invalid Image Type");
    }

    // normalize the whole image before-hand
    // to utilize opencv AVX optimizations
    cv::Mat normalizedImage;
    srcImage.convertTo(normalizedImage, CV_64F, 1.0 / 255.0);

    cv::Mat dstImage(srcImage.rows, srcImage.cols, CV_64FC3);

    for(auto row = 0; row < srcImage.rows; ++row) {
        for(auto col = 0; col < srcImage.cols; ++col) {

            auto lcc = createLCC(normalizedImage.at<cv::Vec3d>(row, col));

            // q = atan2(C′2, C′1).
            const auto theta = std::atan2(lcc.getCrg(), lcc.getCyb());
            const auto theta0 = (theta > 0.0) ? lcc2OrgbAngle(theta) : -lcc2OrgbAngle(-theta);

            // To compute the point (C′yb, C′rg) in oRGB we simply rotate the (C′1, C′2) point:
            //
            //  | C'yb |                   | C'1 |
            //  | C'rg | = R(theta0-theta) | C'2 |
            //
            lcc.rotate(theta0 - theta);

            dstImage.at<cv::Vec3d>(row, col) = lcc.toVec();
        }
    }

    return dstImage;
}

cv::Mat ORGBConverter::getOrgbImage(const cv::Vec3d& scaleFactor, const cv::Vec3d& shiftFactor) const {
    cv::Mat dstImage(orgbImage.rows, orgbImage.cols, CV_8UC3);

    for(auto row = 0; row < orgbImage.rows; ++row) {
        for(auto col = 0; col < orgbImage.cols; ++col) {

            const auto pixel = orgbImage.at<cv::Vec3d>(row, col);
            LCC lcc(pixel[0], pixel[1], pixel[2]);

            lcc.scale(scaleFactor);
            lcc.shift(shiftFactor);

            const auto theta0 = std::atan2(lcc.getCrg(), lcc.getCyb());
            const auto theta = (theta0 > 0) ? Orgb2LccAngle(theta0) : -Orgb2LccAngle(-theta0);

            lcc.inverseRotate(theta0 - theta);

            cv::Vec3b intensity;
            for (int r = 0; r < 3; r++) {
                intensity[2-r] = static_cast<cv::Vec3b::value_type>(std::clamp<double>((
                            InvTransformMat[r][0] * lcc.getLuma() +
                            InvTransformMat[r][1] * lcc.getCyb() +
                            InvTransformMat[r][2] * lcc.getCrg()) * 255, 0.0, 255.0));
            }
            dstImage.at<cv::Vec3b>(row, col) = intensity;
        }
    }

    return dstImage;
}























