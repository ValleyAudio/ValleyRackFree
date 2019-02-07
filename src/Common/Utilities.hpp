#pragma once
#include <cstdint>

template<typename T>
T linterp(T a, T b, T f) {
    return a + f * (b - a);
}

template<typename T>
T clip(T a, T min, T max) {
    return a < min ? min : (a > max ? max : a);
}

template<typename T>
T scale(T a, T inMin, T inMax, T outMin, T outMax) {
    return (a - inMin)/(inMax - inMin) * (outMax - outMin) + outMin;
}

uint32_t mwcRand(uint32_t& w, uint32_t& z);
