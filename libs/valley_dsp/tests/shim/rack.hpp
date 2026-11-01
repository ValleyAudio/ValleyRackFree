#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace rack {
    struct Plugin {};
    struct Model {};

    namespace math {
        using std::clamp;

        template <typename T>
        inline T rescale(T x, T xMin, T xMax, T yMin, T yMax) {
            return yMin + (x - xMin) / (xMax - xMin) * (yMax - yMin);
        }
    }
    using math::clamp;
    using math::rescale;
}
