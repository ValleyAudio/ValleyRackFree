#ifndef TFORM_WAVE_LOADER_HPP
#define TFORM_WAVE_LOADER_HPP
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"
#include "WaveDisplay.hpp"
#include "TFormEditorButton.hpp"
#include "TFormEditorNumberChoice.hpp"

struct TFormBankEditLoadRow : OpaqueWidget {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* cancelButton;
    TFormEditorButton* confirmButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;
    std::shared_ptr<std::vector<std::vector<float>>> detectedWaves;

    int buttonWidth = 44;
    int buttonHeight = 15;
    int buttonOffset = 3;

    std::function<void(int, int)> ingestNewTable;
    std::shared_ptr<Font> font;

    TFormBankEditLoadRow();
    void draw(const DrawArgs& args) override;
    void step() override;
};

#endif
