#include "Valley.hpp"

struct DigitalDisplayColours {
    NVGcolor bgColour = nvgRGB(0x3F, 0x00, 0x00);
    NVGcolor colour = nvgRGB(0xFF, 0x00, 0x00);
    NVGcolor blur1Colour = nvgRGBA(0xFF, 0x5F, 0x5F, 0xBF);
    NVGcolor blur2Colour = nvgRGBA(0xFF, 0x2F, 0x2F, 0xBF);
};

struct DigitalDisplay : TransparentWidget {
    std::string text;
    DigitalDisplayColours colours;
    NVGalign horzAlignment;
    NVGalign vertAlignment;
    int size;

    DigitalDisplay(unsigned long maxDisplayLength = 8);
    void draw(const DrawArgs &args) override;
    void step() override;
    void setText(const std::string& newText);
private:
    std::string backDigits;    
    unsigned long displayLength;
    float blur1 = 10.f;
    float blur2 = 8.f;
};
