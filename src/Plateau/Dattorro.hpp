//
// This plate reverb is based upon Jon Dattorro's 1997 reverb algorithm.
//

#pragma once
#include "../dsp/delays/AllpassFilter.hpp"
#include "../dsp/filters/OnePoleFilters.hpp"
#include "../dsp/modulation/LFO.hpp"
#include <iostream>
#include <array>

class Dattorro1997Tank {
public:
    Dattorro1997Tank(const double initMaxSampleRate = 44100.0,
                     const double initMaxLfoDepth = 0.0,
                     const double initMaxTimeScale = 1.0);

    void process(const double leftInput, const double rightIn,
                 double* leftOut, double* rightOut);

    void freeze(const bool freezeFlag);

    void setSampleRate(const double newSampleRate);
    void setTimeScale(const double newTimeScale);

    void setDecay(const double newDecay);

    void setModSpeed(const double newModSpeed);
    void setModDepth(const double newModDepth);
    void setModShape(const double shape);

    void setHighCutFrequency(const double frequency);
    void setLowCutFrequency(const double frequency);

    void setDiffusion(const double diffusion);

    void clear();

private:
    static constexpr double dattorroSampleRate = 29761.0;
    static constexpr double dattorroSampleTime = 1.0 / dattorroSampleRate;

    static constexpr double leftApf1Time = 672.0;
    static constexpr double leftDelay1Time = 4453.0;
    static constexpr double leftApf2Time = 1800.0;
    static constexpr double leftDelay2Time = 3720.0;

    static constexpr double rightApf1Time = 908.0;
    static constexpr double rightDelay1Time = 4217.0;
    static constexpr double rightApf2Time = 2656.0;
    static constexpr double rightDelay2Time = 3163.0;

    static constexpr size_t leftDelay1RightTap1 = 266;
    static constexpr size_t leftDelay1RightTap2 = 2974;
    static constexpr size_t leftApf2RightTap1 = 1913;
    static constexpr size_t leftDelay2RightTap = 1996;
    static constexpr size_t rightDelay1RightTap = 1990;
    static constexpr size_t rightApf2RightTap = 187;
    static constexpr size_t rightDelay2RightTap = 1066;

    enum LeftOutTaps {
        L_DELAY_1_L_TAP_1,
        L_DELAY_1_L_TAP_2,
        L_APF_2_L_TAP,
        L_DELAY_2_L_TAP,
        R_DELAY_1_L_TAP,
        R_APF_2_L_TAP,
        R_DELAY_2_L_TAP
    };

    enum RightOutTaps {
        R_DELAY_1_R_TAP_1,
        R_DELAY_1_R_TAP_2,
        R_APF_2_R_TAP,
        R_DELAY_2_R_TAP,
        L_DELAY_1_R_TAP,
        L_APF_2_R_TAP,
        L_DELAY_2_R_TAP
    };

    const long kOutputTaps[7] = {266, 2974, 1913, 1996, 1990, 187, 1066};

    static constexpr double maxDiffusion1 = 0.7;
    static constexpr double maxDiffusion2 = 0.7;

    static constexpr double lfoMaxExcursion = 16.0;
    static constexpr double lfo1Freq = 0.10;
    static constexpr double lfo2Freq = 0.150;
    static constexpr double lfo3Freq = 0.120;
    static constexpr double lfo4Freq = 0.180;

    static constexpr double minTimeScale = 0.0001;

    double timePadding = 0.0;

    double scaledLeftApf1Time = leftApf1Time;
    double scaledLeftDelay1Time = leftDelay1Time;
    double scaledLeftApf2Time = leftApf2Time;
    double scaledLeftDelay2Time = leftDelay2Time;

    double scaledRightApf1Time = rightApf1Time;
    double scaledRightDelay1Time = rightDelay1Time;
    double scaledRightApf2Time = rightApf2Time;
    double scaledRightDelay2Time = rightDelay2Time;

    std::array<long, 7> scaledOutputTaps;

    double maxSampleRate = 44100.;
    double sampleRate = maxSampleRate;
    double sampleRateScale = sampleRate / dattorroSampleRate;

    double maxTimeScale = 1.0;
    double timeScale = 1.0;

    double modDepth = 0.0;
    double decayParam = 0.0;
    double decay = 0.0;

    double lfoExcursion = 0.0;

    // Freeze Cross fade
    bool frozen = false;
    double fade = 1.0;
    double fadeTime = 0.002;
    double fadeStep = 1.0 / (fadeTime * sampleRate);
    double fadeDir = 1.0;

    TriSawLFO lfo1;
    TriSawLFO lfo2;
    TriSawLFO lfo3;
    TriSawLFO lfo4;

    double leftSum = 0.0;
    double rightSum = 0.0;

    AllpassFilter<double> leftApf1;
    InterpDelay<double> leftDelay1;
    OnePoleLPFilter leftHighCutFilter;
    OnePoleHPFilter leftLowCutFilter;
    AllpassFilter<double> leftApf2;
    InterpDelay<double> leftDelay2;

    AllpassFilter<double> rightApf1;
    InterpDelay<double> rightDelay1;
    OnePoleLPFilter rightHighCutFilter;;
    OnePoleHPFilter rightLowCutFilter;
    AllpassFilter<double> rightApf2;
    InterpDelay<double> rightDelay2;

    OnePoleHPFilter leftOutDCBlock;
    OnePoleHPFilter rightOutDCBlock;

    void initialiseDelaysAndApfs();

    void tickApfModulation();

    void rescaleApfAndDelayTimes();
    void rescaleTapTimes();
};

class Dattorro {
public:
    Dattorro(const double initMaxSampleRate = 44100.0,
             const double initMaxLfoDepth = 16.0,
             const double initMaxTimeScale = 1.0);
    void process(double leftInput, double rightInput);
    void clear();

    void setTimeScale(double timeScale);
    void setPreDelay(double time);
    void setSampleRate(double sampleRate);

    void freeze(const bool freezeFlag);

    void setInputFilterLowCutoffPitch(double pitch);
    void setInputFilterHighCutoffPitch(double pitch);
    void enableInputDiffusion(bool enable);

    void setDecay(double newDecay);
    void setTankDiffusion(const double diffusion);
    void setTankFilterHighCutFrequency(const double frequency);
    void setTankFilterLowCutFrequency(const double frequency);

    void setTankModSpeed(const double modSpeed);
    void setTankModDepth(const double modDepth);
    void setTankModShape(const double modShape);

    double getLeftOutput() const;
    double getRightOutput() const;

private:
    double preDelayTime = 0.0;
    static constexpr long kInApf1Time = 141;
    static constexpr long kInApf2Time = 107;
    static constexpr long kInApf3Time = 379;
    static constexpr long kInApf4Time = 277;

    static constexpr double dattorroSampleRate = 29761.0;
    double sampleRate = 44100.0;
    double dattorroScaleFactor = sampleRate / dattorroSampleRate;
    double leftSum = 0.0;
    double rightSum = 0.0;

    double rightOut = 0.0;
    double leftOut = 0.0;
    double inputLowCut = 0.0;
    double inputHighCut = 10000.0;
    double reverbHighCut = 10000.0;
    double reverbLowCut = 0.0;
    double modDepth = 1.0;
    double inputDiffusion1 = 0.75;
    double inputDiffusion2 = 0.625;
    double decay = 0.9999;
    double modSpeed = 1.0;
    double diffuseInput = 0.0;

    OnePoleHPFilter leftInputDCBlock;
    OnePoleHPFilter rightInputDCBlock;
    OnePoleLPFilter inputLpf;
    OnePoleHPFilter inputHpf;

    InterpDelay<double> preDelay;

    AllpassFilter<double> inApf1;
    AllpassFilter<double> inApf2;
    AllpassFilter<double> inApf3;
    AllpassFilter<double> inApf4;

    Dattorro1997Tank tank;

    double tankFeed = 0.0;

    double dattorroScale(double delayTime);
};

