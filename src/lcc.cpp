#include "lcc.h"
#include <cmath>

LCC::LCC(const double luma, const double cyb, const double crg)
    : m_luma(luma)
    , m_cyb(cyb)
    , m_crg(crg)
{

}

void LCC::rotate(const double theta) {
//       Rotation Matrix
//
//     | cos(x) -sin(x) |
//     | sin(x)  cos(x) |
//
    const auto cosAngle = cos(theta);
    const auto sinAngle = sin(theta);

    const auto newCyb = cosAngle * m_cyb - sinAngle * m_crg;
    const auto newCrg = sinAngle * m_cyb + cosAngle * m_crg;

    m_cyb = newCyb;
    m_crg = newCrg;
}

void LCC::inverseRotate(const double theta) {
//      Inverse Rotation Matrix
//
//      |  cos(x) sin(x) |
//      | -sin(x) cos(x) |
//
    const auto cosAngle = cos(theta);
    const auto sinAngle = sin(theta);

    const auto newCyb =  cosAngle * m_cyb + sinAngle * m_crg;
    const auto newCrg = -sinAngle * m_cyb + cosAngle * m_crg;

    m_cyb = newCyb;
    m_crg = newCrg;
}

cv::Vec3d LCC::toVec() const {
    return { m_luma, m_cyb, m_crg };
}

void LCC::scale(const cv::Vec3d& scaler) {
    m_luma *= scaler[0];
    m_cyb  *= scaler[1];
    m_crg  *= scaler[2];
}

void LCC::shift(const cv::Vec3d& shifter) {
    m_luma += shifter[0];
    m_cyb  += shifter[1];
    m_crg  += shifter[2];
}
