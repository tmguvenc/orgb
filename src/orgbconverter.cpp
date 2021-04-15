#include "orgbconverter.h"
#include "angletransformer.h"

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

ORGBConverter::ORGBConverter()
{
    transformMatrix.create(3, 3, CV_64FC1);
    invTransformMatrix.create(3, 3, CV_64FC1);
    memcpy(transformMatrix.data, TransformMat, sizeof (TransformMat));
    memcpy(invTransformMatrix.data, InvTransformMat, sizeof (InvTransformMat));
}

//struct LCC {
//    double luma{};
//    double cyb{};
//    double crg{};
//};

//LCC lccConvert(const cv::Vec3d& scaledPixel) {
//    double lcc[3];

//    for (int r = 0; r < 3; r++) {
//        lcc[r] = 0;
//        for (int c = 0; c < 3; c++) {
//            lcc[r] += TransformMat[r][c] * scaledPixel[c];
//        }
//    }

//    return {lcc[0], lcc[1], lcc[2]};
//}

cv::Mat ORGBConverter::convert(const cv::Mat& srcImage) {
    if(srcImage.type() != CV_8UC3) {
        throw std::runtime_error("Invalid Image Type");
    }

    // for cache efficiency, we normalize the whole image before-hand.
    cv::Mat normalizedImage;
    srcImage.convertTo(normalizedImage, CV_64F, 1.0 / 255.0);

    for(auto row = 0; row < srcImage.rows; ++row) {
        for(auto col = 0; col < srcImage.cols; ++col) {

//            const auto lcc = lccConvert(normalizedImage.at<cv::Vec3d>(row, col));
            const cv::Mat lcc = transformMatrix.mul(normalizedImage.at<cv::Vec3d>(row, col));

            // q = atan2(C′2, C′1).
            const auto cyb = lcc.at<double>(1);
            const auto crg = lcc.at<double>(2);
            const auto theta = std::atan2(crg, cyb);
            const auto theta0 = (theta > 0.0) ? lcc2OrgbAngle(theta) : -Orgb2LccAngle(-theta);

//          To compute the point (C′yb,C′rg) in oRGB we simply rotate the (C′1 ,C′2) point:
//
//           | C'yb |                   | C'1 |
//           | C'rg | = R(theta0-theta) | C'2 |
//
            const auto angle = theta0 - theta;

            //Point rotation
            const auto rotateMatrixCos = cos(angle);
            const auto rotateMatrixSin = sin(angle);
            const auto Cyb = rotateMatrixCos*cyb - rotateMatrixSin*crg;
            const auto Crg = rotateMatrixSin*cyb + rotateMatrixCos*crg;
            oRGB[i][j][1] = Cyb;
            oRGB[i][j][2] = Crg;
        }
    }

    return cv::Mat();
}
