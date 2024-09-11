#include <cmath>

namespace DSP {

inline float Interpolate(const float* table, float index, float size) {
    index *= size;
    // MAKE_INTEGRAL_FRACTIONAL(index)
    const float m { std::fmax(index, 0.f) }; // structure [0, 1]
    const int mInt { static_cast<int>(std::floor(m)) };
    const float mFrac { m - mInt };

    float a = table[mInt];
    float b = table[mInt + 1];

    return a + (b - a) * mFrac;
}


}