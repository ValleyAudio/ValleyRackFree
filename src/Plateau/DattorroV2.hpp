/**
 * @file DattorroV2.hpp
 * @author Dale Johnson
 * @date 24/04/2021
 * @brief A revamped version of the 'Plate Reverb' algorithm proposed by Dattorro (1995) as used
 * in Plateau. Now features block processing to allow for more efficient operation.
 */

#pragma once

#include "../Common/DSP/AllpassFilter.hpp"
#include "../Common/DSP/MultiTapAllpassFilter.hpp"
#include "../Common/DSP/OnePoleFilters.hpp"
#include "../Common/DSP/LFO.hpp"
#include <cstdint>
#include <array>

class DattorroV2 {
public:
    DattorroV2(double initSampleRate = 44100.0, uint64_t initBlockSize = 256);

    void blockProcess(const double* leftInBuffer, const double* rightInBuffer,
                      double* leftOutBuffer, double* rightOutBuffer);
    void clear();

private:
    uint64_t blockSize = 1;
    double decay = 0.7071;
    double tankLeftSum = 0.0;
    double tankRightSum = 0.0;
    double leftOutputSum = 0.0;
    double rightOutputSum = 0.0;
    OnePoleLPFilter inputLpf;
    OnePoleHPFilter inputHpf;
    AllpassFilter<double> inApf1, inApf2, inApf3, inApf4;

    AllpassFilter<double> leftApf1;
    MultiTapAllpassFilter<double, 3> leftApf2;
    MultiTapInterpDelay<double, 4> leftDelay1;
    MultiTapInterpDelay<double, 3> leftDelay2;

    AllpassFilter<double> rightApf1;
    MultiTapAllpassFilter<double, 3> rightApf2;
    MultiTapInterpDelay<double, 4> rightDelay1;
    MultiTapInterpDelay<double, 3> rightDelay2;

    TriSawLFO leftApf1Lfo, leftApf2Lfo;
    TriSawLFO rightApf1Lfo, rightApf2Lfo;

    std::vector<double> inputChainBuffer;
};
