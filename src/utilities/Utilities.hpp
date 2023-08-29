#pragma once
#include <cstdint>
#include <string>
#include <cmath>

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

template<typename T>
T semitone(T x) {
    return ((int)(x * 12)) * 0.0833333f;
}

float pitch2freq(const float pitch);

uint32_t mwcRand(uint32_t& w, uint32_t& z);

std::string extractDirectoryFromFilePath(const std::string& filepath); 
