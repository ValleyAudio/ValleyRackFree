/**
 * @file MultiTapInterpDelay.hpp
 * @author Dale Johnson, Valley Audio Soft
 * @brief A more optimised version of the linear interpolating delay.
 */

#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include "../../utilities/Utilities.hpp"

template<typename T = float, uint64_t numTaps = 1>
class MultiTapInterpDelay {
public:
    T input = T(0);
    T output[numTaps];

    MultiTapInterpDelay(uint64_t maxLength = 512, T initDelayTime = 0) {
        l = maxLength;
        buffer = std::vector<T>(l, T(0));

        for (uint64_t tap = 0; tap < numTaps; ++tap) {
            output[tap] = T(0);
            f[tap] = T(0);
            t[tap] = 0;
        }

        std::array<T, numTaps> initDelayTimes;
        initDelayTimes.fill(initDelayTime);
        setDelayTimes(initDelayTimes);
    }

    MultiTapInterpDelay(uint64_t maxLength, const std::array<T, numTaps>& initDelayTimes) {
        l = maxLength;
        buffer = std::vector<T>(l, T(0));

        for (uint64_t tap = 0; tap < numTaps; ++tap) {
            output[tap] = T(0);
            f[tap] = T(0);
            t[tap] = 0;
        }

        setDelayTimes(initDelayTimes);
    }

    void process() {
        buffer[w] = input;
        for (uint64_t tap = 0; tap < numTaps; ++tap) {
            int64_t r = w - t[tap];
            if (r < 0) {
                r += l;
            }

            int64_t upperR = r - 1;
            if (upperR < 0) {
                upperR += l;
            }
            output[tap] = linterp(buffer[r], buffer[upperR], f[tap]);
        }

        ++w;
        if (w == l) {
            w = 0;
        }
    }

    void setDelayTimes(const std::array<T, numTaps>& newDelayTimes) {
        for (size_t tap = 0; tap < numTaps; ++tap) {
            T newT = newDelayTimes[tap];
            t[tap] = static_cast<int64_t>(newT);
            f[tap] = newT - static_cast<T>(t[tap]);
        }
    }

    void setDelayTime(uint64_t tap, T newDelayTime) {
        if (newDelayTime >= l) {
            newDelayTime = l - 1;
        }
        if (newDelayTime < 0) {
            newDelayTime = 0;
        }
        t[tap] = static_cast<int64_t>(newDelayTime);
        f[tap] = newDelayTime - static_cast<T>(t[tap]);
    }

    void clear() {
        std::fill(buffer.begin(), buffer.end(), T(0));
        input = T(0);
        for (auto& o : output) {
            o = T(0);
        }
    }

private:
    int64_t w = 0;
    int64_t t[numTaps];
    T f[numTaps];
    int64_t l = 512;
    std::vector<T> buffer;
};
