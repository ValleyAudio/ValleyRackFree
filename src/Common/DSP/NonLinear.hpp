/**
 * @filename NonLinear.hpp
 * @author Dale Johnson
 */

#pragma once

/** Tanh signal distortion using piecewise approximation of a hyperbolic tangent */
inline float tanhDriveSignal(float x, float drive) {
    x *= drive;

    if(x < -1.25f) {
        return -1.f;
    }
    else if(x < -0.75f) {
        return 1.f - (x * (-2.5f - x) - 0.5625f) - 1.f;
    }
    else if(x > 1.25f) {
        return 1.f;
    }
    else if(x > 0.75f) {
        return x * (2.5f - x) - 0.5625f;
    }

    return x;
}
