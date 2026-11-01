#include <catch2/catch_test_macros.hpp>
#include "AudioReferenceCommon.hpp"
#include <filesystem>
#include <cmath>
#include <iostream>

namespace fs = std::filesystem;

static std::string resolveReferenceFilePath(const std::string& filename) {
    std::vector<fs::path> searchDirs;
#ifdef REFERENCE_DIR
    searchDirs.push_back(fs::path(REFERENCE_DIR));
#endif
    searchDirs.push_back(fs::current_path() / "reference");
    searchDirs.push_back(fs::current_path() / "tests" / "reference");
    searchDirs.push_back(fs::current_path() / "libs" / "valley_dsp" / "tests" / "reference");

    for (const auto& dir : searchDirs) {
        fs::path p = dir / filename;
        if (fs::exists(p)) {
            return p.string();
        }
    }
    // Return path in default dir if not found (will throw on read with clear message)
#ifdef REFERENCE_DIR
    return (fs::path(REFERENCE_DIR) / filename).string();
#else
    return (fs::current_path() / "reference" / filename).string();
#endif
}

TEST_CASE("Audio Reference Regression Tests", "[reference]") {
    const auto& processors = valley::test::getProcessors();
    const auto& sampleRates = valley::test::getSampleRates();
    const valley::test::TestSignalType signalTypes[] = {
        valley::test::TestSignalType::Impulse,
        valley::test::TestSignalType::Sweep
    };

    for (const auto& proc : processors) {
        for (const auto& sigType : signalTypes) {
            std::string sigName = (sigType == valley::test::TestSignalType::Impulse) ? "impulse" : "sweep";

            for (int sr : sampleRates) {
                DYNAMIC_SECTION(proc.name << " | " << sigName << " | " << sr << " Hz") {
                    std::string filename = valley::test::getSnapshotFilename(proc.name, sigType, sr);
                    std::string path = resolveReferenceFilePath(filename);

                    // Compute current output
                    std::vector<float> current = valley::test::runProcessor(proc.name, sigType, sr);

                    // Read expected reference snapshot
                    std::vector<float> expected;
                    REQUIRE_NOTHROW([&]() {
                        expected = valley::test::readBinaryFloats(path);
                    }());

                    REQUIRE(current.size() == expected.size());
                    REQUIRE(current.size() > 0);

                    float maxAbsDiff = 0.0f;
                    for (size_t i = 0; i < current.size(); ++i) {
                        float c = current[i];
                        float e = expected[i];

                        // Assert no NaN or Inf
                        REQUIRE_FALSE(std::isnan(c));
                        REQUIRE_FALSE(std::isinf(c));

                        float diff = std::abs(c - e);
                        if (diff > maxAbsDiff) {
                            maxAbsDiff = diff;
                        }
                    }

                    // Assert null-test within 1e-6f
                    REQUIRE(maxAbsDiff <= 1e-6f);
                }
            }
        }
    }
}
