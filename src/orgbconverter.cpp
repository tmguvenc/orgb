#include "orgbconverter.h"
#include "angletransformer.h"
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

struct LCC {
    double luma{};
    double cyb{};
    double crg{};
};

using Rotated = std::pair<double, double>;

Rotated rotate(const LCC& lcc, const double angle) {
    //          Rotation Matrix
    //
    //          R = | cos(x) -sin(x) |
    //              | sin(x)  cos(x) |
    //
    const auto cosAngle = cos(angle);
    const auto sinAngle = sin(angle);
    return {
        cosAngle * lcc.cyb - sinAngle * lcc.crg,
        sinAngle * lcc.cyb + cosAngle * lcc.crg
    };



    /*
     *
            double rotateMatrixCos = cos(angle);
            double rotateMatrixSin = sin(angle);
            LCbyCgrScaledVector[1] = rotateMatrixCos * newCyb + rotateMatrixSin * newCrg;
            LCbyCgrScaledVector[2] = rotateMatrixCos * newCrg - rotateMatrixSin * newCyb;

     */
}

Rotated inverseRotate(const LCC& lcc, const double angle) {
    //          Inverse Rotation Matrix
    //
    //          R = |  cos(x) sin(x) |
    //              | -sin(x) cos(x) |
    //
    const auto cosAngle = cos(angle);
    const auto sinAngle = sin(angle);
    return {
         cosAngle * lcc.cyb + sinAngle * lcc.crg,
        -sinAngle * lcc.cyb + cosAngle * lcc.crg
    };
}

LCC lccConvert(const cv::Vec3d& scaledPixel) {
    double lcc[3];

    for (int r = 0; r < 3; r++) {
        lcc[r] = 0;
        for (int c = 0; c < 3; c++) {
            lcc[r] += TransformMat[r][c] * scaledPixel[c];
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

            //const cv::Mat lcc = transformMatrix.mul(normalizedImage.at<cv::Vec3d>(row, col));
            const auto lcc = lccConvert(normalizedImage.at<cv::Vec3d>(row, col));
            auto& orgbPixel = dstImage.at<cv::Vec3d>(row, col);

            // luma
            orgbPixel[0] = lcc.luma;

            // q = atan2(C′2, C′1).
            const auto theta = std::atan2(lcc.crg, lcc.cyb);
            const auto theta0 = (theta > 0.0) ? lcc2OrgbAngle(theta) : -Orgb2LccAngle(-theta);

//          To compute the point (C′yb, C′rg) in oRGB we simply rotate the (C′1, C′2) point:
//
//           | C'yb |                   | C'1 |
//           | C'rg | = R(theta0-theta) | C'2 |
//
            std::tie(orgbPixel[1], orgbPixel[2]) = rotate(lcc, theta0 - theta);
        }
    }

    return dstImage;
}

cv::Mat ORGBConverter::getOrgbImage(const LCC& scaleFactor, const LCC& shiftFactor) const {
    cv::Mat dstImage(orgbImage.rows, orgbImage.cols, CV_8UC3);

    for(auto row = 0; row < orgbImage.rows; ++row) {
        for(auto col = 0; col < orgbImage.cols; ++col) {

            const auto pixel = orgbImage.at<cv::Vec3d>(row, col);

            const auto newLuma = scaleFactor.luma * pixel[0] + shiftFactor.luma;
            const auto newCyb = scaleFactor.cyb * pixel[1] + shiftFactor.cyb;
            const auto newCrg = scaleFactor.crg * pixel[2] + shiftFactor.crg;

            const auto theta0 = atan2(newCrg, newCyb);
            const auto theta = (theta0 > 0) ? Orgb2LccAngle(theta0) : -Orgb2LccAngle(-theta0);
            const auto angle = theta0 - theta;
            const auto [cyb, crg] = inverseRotate({newLuma, newCyb, newCrg}, angle);

            cv::Vec3b intensity;
            for (int r = 0; r < 3; r++) {
                intensity[2 - r] = static_cast<uint8_t>(std::clamp<double>((
                            InvTransformMat[r][0] * newLuma +
                            InvTransformMat[r][1] * cyb +
                            InvTransformMat[r][2] * crg) * 255, 0.0, 255.0));
            }
            dstImage.at<cv::Vec3b>(row, col) = intensity;
        }
    }

    return dstImage;
}























