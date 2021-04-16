#ifndef LCC_H
#define LCC_H

#include <opencv2/core/types.hpp>

class LCC
{
public:
    LCC(const double luma, const double cyb, const double crg);
    ~LCC() = default;

    void rotate(const double theta);
    void inverseRotate(const double theta);

    void transform(const cv::Vec3d& scaledPixel);
    void scale(const cv::Vec3d& scaler);
    void shift(const cv::Vec3d& shifter);

    inline double getLuma() const noexcept { return m_luma; }
    inline double getCyb() const noexcept { return m_cyb; }
    inline double getCrg() const noexcept { return m_crg; }

    cv::Vec3d toVec() const;

    LCC(const LCC& other) = delete;
    LCC& operator=(const LCC& other) = delete;

    LCC(LCC&& other) = delete;
    LCC& operator=(LCC&& other) = delete;

private:
    double m_luma;
    double m_cyb;
    double m_crg;
};

#endif // LCC_H
