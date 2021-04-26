/**
 * @file DattorroV2.hpp
 * @author Dale Johnson
 * @date 24/04/2021
 * @brief A revamped version of the 'Plate Reverb' algorithm proposed by Dattorro (1995) as used
 * in Plateau. Now features block processing to allow for more efficient operation.
 */

#pragma once

#include "../Common/DSP/AllpassFilter.hpp"
#include "../Common/DSP/OnePoleFilters.hpp"
#include "../Common/DSP/LFO.hpp"
#include <cstdint>
#include <array>

class DattorroV2 {
public:
    DattorroV2(double sampleRate = 44100.0, uint64_t blockSize = 256);

    void blockProcess(const double* leftInBuffer, const double* rightInBuffer,
                      double* leftOutBuffer, double* rightOutBuffer);
    void clear();

private:
    std::array<double, 256> inputChainBuffer = {0.0};
};
