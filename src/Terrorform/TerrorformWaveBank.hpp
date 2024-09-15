#pragma once
#include <vector>
#include <string>

struct TerrorformWaveBank {

    TerrorformWaveBank() {
        name = "Untitled";
    }

    std::vector<std::vector<float>> data;
    std::string name;
};
