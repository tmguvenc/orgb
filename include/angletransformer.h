#ifndef ANGLETRANSFORMER_H
#define ANGLETRANSFORMER_H

#include <numeric>

static constexpr double PI = 22.0 / 7.0;

static inline constexpr double lcc2OrgbAngle(const double angle) {
    // theta < pi/3
    if (angle < PI / 3.0) {
        // (3/2)*theta
        return 3.0 / 2.0 * angle;
    }

    // pi >= theta >= pi/3
    if (angle <= PI) {
        // pi/2 + (3/4)(theta-pi/3)
        return (PI / 2.0) + (3.0 / 4.0)*(angle - PI / 3.0);
    }

    return -std::numeric_limits<double>::min();
}

static inline double Orgb2LccAngle(const double angle) {

    // theta < pi/2
    if (angle < PI / 2.0) {
        // (2/3) * theta
        return 2.0 / 3.0 * angle;
    }

    // pi >= theta >= pi/2
    if (angle <= PI) {

        // pi/3 + (4/3)(theta - pi/2)
        return PI / 3.0 + (4.0 / 3.0)*(angle - PI / 2);
    }

    return -std::numeric_limits<double>::min();
}

#endif // ANGLETRANSFORMER_H
