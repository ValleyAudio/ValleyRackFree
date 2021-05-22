/**
 * @file DattorroV2.hpp
 * @author Dale Johnson
 * @date 24/04/2021
 * @brief A revamped version of the 'Plate Reverb' algorithm proposed by Dattorro (1995) as used
 * in Plateau. Now features block processing to allow for more efficient operation.
 */

/* TODO
 * - Add positive and negative 'plate diffusion' to assign to tank APFs
 * - Add setters for absorption filters
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
    double plateDiffusion1 = 0.7071;
    double plateDiffusion2 = 0.5;
    bool diffuseInput = true;

    DattorroV2(double initSampleRate = 44100.0, uint64_t initBlockSize = 256);

    void blockProcess(const double* leftInBuffer, const double* rightInBuffer,
                      double* leftOutBuffer, double* rightOutBuffer, uint64_t blockSize);
    void clear();

    void freeze(bool freezing);

    void setDecay(double newDecay);

    void setSize(double newSize);

    void setSizeTrajectory(const std::vector<double>& newSizeTrajectory);

    void setAbsorption(double inputLow, double inputHigh, double tankLow, double tankHigh);

private:
    // Initial delay times
    const double dattoroSampleRate = 29761.0;

    const uint64_t kInApf1Time = 141;
    const uint64_t kInApf2Time = 107;
    const uint64_t kInApf3Time = 379;
    const uint64_t kInApf4Time = 277;

    const uint64_t kLeftApf1Time = 672;
    const uint64_t kRightApf1Time = 908;

    // Delay taps
    const std::array<double, 4> kLeftDelay1Taps = {{4451, 266, 2974, 1990}};
    const std::array<double, 3> kLeftDelay2Taps = {{3720, 1996, 1066}};
    const std::array<double, 4> kRightDelay1Taps = {{4217, 1990, 266, 2974}};
    const std::array<double, 3> kRightDelay2Taps = {{3163, 1066, 1996}};
    const std::array<double, 3> kLeftApf2Taps = {{1800, 1913, 187}};
    const std::array<double, 3> kRightApf2Taps = {{2656, 187, 1913}};

    double scaledLeftApf1Time, scaledRightApf1Time;

    std::array<double, 4> leftDelay1Taps;
    std::array<double, 3> leftDelay2Taps;
    std::array<double, 4> rightDelay1Taps;
    std::array<double, 3> rightDelay2Taps;
    std::array<double, 3> leftApf2Taps;
    std::array<double, 3> rightApf2Taps;

    uint64_t maxBlockSize = 1;
    double decay = 0.7071;
    double tankLeftSum = 0.0;
    double tankRightSum = 0.0;
    double lfoExcursion = 1.0;
    double size = 1.0;
    const double minSize = 0.0;
    const double maxSize = 4.0;

    // Freeze mode
    double thawedDecay = 0.0;
    double freezeXFade = 0.0;
    double freezeXFadeStepSize = 0.0;
    double freezeXFadeDir = -1.0;
    bool frozen = false;

    std::vector<double> sizeTrajectory;
    std::vector<double> inputChainBuffer;

    // Parameter smoothing
    OnePoleLPFilter decaySmoother;
    OnePoleLPFilter sizeSmoother;

    // The reverb guts
    OnePoleLPFilter inputLpf;
    OnePoleHPFilter inputHpf;
    AllpassFilter<double> inApf1, inApf2, inApf3, inApf4;

    AllpassFilter<double> leftApf1;
    MultiTapInterpDelay<double, 4> leftDelay1;
    OnePoleHPFilter leftHpf;
    OnePoleLPFilter leftLpf;
    MultiTapAllpassFilter<double, 3> leftApf2;
    MultiTapInterpDelay<double, 3> leftDelay2;

    AllpassFilter<double> rightApf1;
    MultiTapInterpDelay<double, 4> rightDelay1;
    OnePoleHPFilter rightHpf;
    OnePoleLPFilter rightLpf;
    MultiTapAllpassFilter<double, 3> rightApf2;
    MultiTapInterpDelay<double, 3> rightDelay2;

    TriSawLFO leftApf1Lfo, leftApf2Lfo;
    TriSawLFO rightApf1Lfo, rightApf2Lfo;


};
