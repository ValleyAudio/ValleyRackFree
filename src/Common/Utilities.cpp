#include "Utilities.hpp"

uint32_t mwcRand(uint32_t& w, uint32_t& z) {
    z = 36969 * (z & 65535) + (z >> 16);
    w = 18000 * (w & 65535) + (w >> 16);
    return (z << 16) + w;
}

std::string extractDirectoryFromFilePath(const std::string& filepath) {
    std::string directory = "";

    size_t lastSlashIdx;
    #ifdef _WIN32
        lastSlashIdx = filepath.rfind('\\');
    #elif __APPLE__
        lastSlashIdx = filepath.rfind('/');
    #elif __linux__
        lastSlashIdx = filepath.rfind('/');
    #endif

    if (std::string::npos != lastSlashIdx) {
        directory = filepath.substr(0, lastSlashIdx);
    }
    return directory;
}
