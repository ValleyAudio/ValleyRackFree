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

    void freeze(bool freezing);

private:
    uint64_t blockSize = 1;
    double decay = 0.7071;
    double tankLeftSum = 0.0;
    double tankRightSum = 0.0;
    double lfoExcursion = 1.0;

    // Initial delay times
    const uint64_t kInApf1Time = 141;
    const uint64_t kInApf2Time = 107;
    const uint64_t kInApf3Time = 379;
    const uint64_t kInApf4Time = 277;

    const uint64_t kLeftApf1Time = 672;
    const uint64_t kLeftApf2Time = 1800;
    const uint64_t kLeftDelay1Time = 4453;
    const uint64_t kLeftDelay2Time = 3720;

    const uint64_t kRightApf1Time = 908;
    const uint64_t kRightApf2Time = 2656;
    const uint64_t kRightDelay1Time = 4217;
    const uint64_t kRightDelay2Time = 3163;

    // Delay taps
    const std::array<double, 4> leftDelay1Taps = {4451, 266, 2974, 1990};
    const std::array<double, 3> leftDelay2Taps = {3720, 1996, 1066};
    const std::array<double, 4> rightDelay1Taps = {4217, 1990, 266, 2974};
    const std::array<double, 3> rightDelay2Taps = {3163, 1066, 1996};
    const std::array<double, 3> leftApf2Taps = {1800, 1913, 187};
    const std::array<double, 3> rightApf2Taps = {2656, 187, 1913};

    OnePoleLPFilter inputLpf;
    OnePoleHPFilter inputHpf;
    AllpassFilter<double> inApf1, inApf2, inApf3, inApf4;

    AllpassFilter<double> leftApf1;
    MultiTapInterpDelay<double, 4> leftDelay1;
    OnePoleHPFilter leftHighpassFilter;
    OnePoleLPFilter leftLowpassFilter;
    MultiTapAllpassFilter<double, 3> leftApf2;
    MultiTapInterpDelay<double, 3> leftDelay2;

    AllpassFilter<double> rightApf1;
    MultiTapInterpDelay<double, 4> rightDelay1;
    OnePoleHPFilter rightHighpassFilter;
    OnePoleLPFilter rightLowpassFilter;
    MultiTapAllpassFilter<double, 3> rightApf2;
    MultiTapInterpDelay<double, 3> rightDelay2;

    TriSawLFO leftApf1Lfo, leftApf2Lfo;
    TriSawLFO rightApf1Lfo, rightApf2Lfo;

    std::vector<double> inputChainBuffer;

    // Freeze mode
    double thawedDecay = 0.0;
    double freezeXFade = 0.0;
    double freezeXFadeStepSize = 0.0;
    double freezeXFadeDir = -1.0;
    bool frozen = false;
};
