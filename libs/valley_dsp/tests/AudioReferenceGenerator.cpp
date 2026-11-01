#include "AudioReferenceCommon.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    std::string outputDir = "reference";
#ifdef REFERENCE_DIR
    outputDir = REFERENCE_DIR;
#endif
    if (argc > 1) {
        outputDir = argv[1];
    }

    std::cout << "[AudioReferenceGenerator] Writing reference audio to: " << outputDir << std::endl;
    fs::create_directories(outputDir);

    const auto& processors = valley::test::getProcessors();
    const auto& sampleRates = valley::test::getSampleRates();
    const valley::test::TestSignalType signalTypes[] = {
        valley::test::TestSignalType::Impulse,
        valley::test::TestSignalType::Sweep
    };

    int generatedCount = 0;
    for (const auto& proc : processors) {
        for (const auto& sigType : signalTypes) {
            for (int sr : sampleRates) {
                std::string filename = valley::test::getSnapshotFilename(proc.name, sigType, sr);
                std::string fullPath = (fs::path(outputDir) / filename).string();

                std::vector<float> audio = valley::test::runProcessor(proc.name, sigType, sr);
                valley::test::writeBinaryFloats(fullPath, audio);
                std::cout << "  Generated: " << filename << " (" << audio.size() << " samples)" << std::endl;
                ++generatedCount;

                if (!proc.aliasName.empty()) {
                    std::string aliasFilename = valley::test::getSnapshotFilename(proc.aliasName, sigType, sr);
                    std::string aliasFullPath = (fs::path(outputDir) / aliasFilename).string();
                    valley::test::writeBinaryFloats(aliasFullPath, audio);
                    std::cout << "  Generated alias: " << aliasFilename << " (" << audio.size() << " samples)" << std::endl;
                    ++generatedCount;
                }
            }
        }
    }

    std::cout << "[AudioReferenceGenerator] Successfully generated " << generatedCount << " snapshot files." << std::endl;
    return 0;
}
