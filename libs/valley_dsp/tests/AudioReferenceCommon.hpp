#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdint>

// Direct inclusion of existing unmodified DSP headers
#include "dsp/filters/OnePoleFilters.hpp"
#include "dsp/filters/OTAFilter.hpp"
#include "dsp/delays/InterpDelay.hpp"
#include "dsp/delays/AllpassFilter.hpp"
#include "dsp/generators/DOsc.hpp"
#include "Plateau/Dattorro.hpp"
#include "Amalgam/VecAmalgam.hpp"

namespace valley::test {

enum class TestSignalType {
    Impulse,
    Sweep
};

struct ProcessorInfo {
    std::string name;
    std::string aliasName; // optional alias e.g. state_variable_filter
};

inline const std::vector<ProcessorInfo>& getProcessors() {
    static const std::vector<ProcessorInfo> processors = {
        {"one_pole_lp", ""},
        {"one_pole_hp", ""},
        {"ota_filter", "state_variable_filter"},
        {"interp_delay", ""},
        {"allpass_filter", ""},
        {"d_osc", ""},
        {"dattorro", ""},
        {"vec_amalgam", ""}
    };
    return processors;
}

inline const std::vector<int>& getSampleRates() {
    static const std::vector<int> sampleRates = {44100, 48000};
    return sampleRates;
}

// 1.0f Dirac unit impulse followed by 4096 zero samples (total 4097 samples)
inline std::vector<float> generateImpulseSignal() {
    std::vector<float> buffer(4097, 0.0f);
    buffer[0] = 1.0f;
    return buffer;
}

// 4096-sample logarithmic sine and modulation sweep
inline std::vector<float> generateSweepSignal(double sampleRate) {
    constexpr size_t numSamples = 4096;
    std::vector<float> buffer(numSamples, 0.0f);
    constexpr double f0 = 20.0;
    const double f1 = std::min(20000.0, 0.45 * sampleRate);
    const double ratio = f1 / f0;
    const double logRatio = std::log(ratio);
    constexpr double pi = 3.14159265358979323846;

    for (size_t n = 0; n < numSamples; ++n) {
        double tRatio = static_cast<double>(n) / static_cast<double>(numSamples);
        double phase = 2.0 * pi * f0 * (static_cast<double>(numSamples) / (logRatio * sampleRate)) *
                       (std::pow(ratio, tRatio) - 1.0);
        buffer[n] = static_cast<float>(std::sin(phase));
    }
    return buffer;
}

inline std::vector<float> runProcessor(const std::string& processorName,
                                      TestSignalType signalType,
                                      int sampleRate) {
    const std::vector<float> input = (signalType == TestSignalType::Impulse)
                                         ? generateImpulseSignal()
                                         : generateSweepSignal(sampleRate);
    std::vector<float> output(input.size(), 0.0f);

    if (processorName == "one_pole_lp") {
        OnePoleLPFilter filter(1000.0, sampleRate);
        for (size_t i = 0; i < input.size(); ++i) {
            filter.input = input[i];
            output[i] = static_cast<float>(filter.process());
        }
    } else if (processorName == "one_pole_hp") {
        OnePoleHPFilter filter(500.0, sampleRate);
        for (size_t i = 0; i < input.size(); ++i) {
            filter.input = input[i];
            output[i] = static_cast<float>(filter.process());
        }
    } else if (processorName == "ota_filter" || processorName == "state_variable_filter") {
        OTAFilter filter;
        filter.setSampleRate(static_cast<float>(sampleRate));
        filter.setCutoff(0.0f);
        filter.setQ(0.5f);
        filter.set4Pole(1.0f);
        for (size_t i = 0; i < input.size(); ++i) {
            output[i] = filter.process(input[i]);
        }
    } else if (processorName == "interp_delay") {
        InterpDelay<float> delay(2048, 256);
        delay.setDelayTime(128.5f);
        for (size_t i = 0; i < input.size(); ++i) {
            delay.input = input[i];
            delay.process();
            output[i] = delay.output;
        }
    } else if (processorName == "allpass_filter") {
        AllpassFilter<float> apf(2048, 256, 0.7f);
        for (size_t i = 0; i < input.size(); ++i) {
            apf.input = input[i];
            output[i] = apf.process();
        }
    } else if (processorName == "d_osc") {
        DOsc osc;
        osc.setSampleRate(static_cast<float>(sampleRate));
        osc.setSubWave(1);
        osc.setSubOctave(DOsc::SUB_OCTAVE_HARMONIC);
        if (signalType == TestSignalType::Impulse) {
            osc.setFrequency(220.0f);
            for (size_t i = 0; i < input.size(); ++i) {
                osc.process();
                output[i] = osc.saw;
            }
        } else {
            for (size_t i = 0; i < input.size(); ++i) {
                float freq = 20.0f * std::pow(100.0f, static_cast<float>(i) / static_cast<float>(input.size()));
                osc.setFrequency(freq);
                osc.process();
                output[i] = osc.saw;
            }
        }
    } else if (processorName == "dattorro") {
        Dattorro reverb(sampleRate);
        reverb.setSampleRate(sampleRate);
        reverb.setPreDelay(0.01);
        reverb.setDecay(0.7);
        reverb.setInputFilterLowCutoffPitch(-2.0);
        reverb.setInputFilterHighCutoffPitch(4.0);
        for (size_t i = 0; i < input.size(); ++i) {
            reverb.process(static_cast<double>(input[i]), static_cast<double>(input[i]));
            output[i] = static_cast<float>(reverb.getLeftOutput());
        }
    } else if (processorName == "vec_amalgam") {
        VecAmalgam amalgam;
        amalgam.setSampleRate(static_cast<float>(sampleRate));
        amalgam.setMode(VecAmalgam::RING_MOD_1_MODE);
        for (size_t i = 0; i < input.size(); ++i) {
            __m128 vx = _mm_set1_ps(input[i]);
            __m128 vy = (signalType == TestSignalType::Impulse)
                            ? _mm_set1_ps(0.5f)
                            : _mm_set1_ps(0.8f * std::cos(2.0f * 3.14159265f * 440.0f *
                                                          static_cast<float>(i) /
                                                          static_cast<float>(sampleRate)));
            __m128 vOut = amalgam.process(vx, vy, 0.4f, 0.3f);
            output[i] = _mm_cvtss_f32(vOut);
        }
    } else {
        throw std::invalid_argument("Unknown processor name: " + processorName);
    }

    return output;
}

inline void writeBinaryFloats(const std::string& path, const std::vector<float>& data) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open file for writing: " + path);
    }
    out.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(float));
}

inline std::vector<float> readBinaryFloats(const std::string& path) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in) {
        throw std::runtime_error("Failed to open file for reading: " + path);
    }
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<float> data(size / sizeof(float));
    in.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

inline std::string getSnapshotFilename(const std::string& procName,
                                      TestSignalType sigType,
                                      int sampleRate) {
    std::string testStr = (sigType == TestSignalType::Impulse) ? "impulse" : "sweep";
    return procName + "_" + testStr + "_" + std::to_string(sampleRate) + ".bin";
}

} // namespace valley::test
