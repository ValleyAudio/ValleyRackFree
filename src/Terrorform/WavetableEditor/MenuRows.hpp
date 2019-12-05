#ifndef TFORM_EDITOR_MENU_ROWS
#define TFORM_EDITOR_MENU_ROWS
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"
#include "TFormEditorButton.hpp"
#include "TFormEditorNumberChoice.hpp"
#include "WaveDisplay.hpp"

struct TFormMenuRow : OpaqueWidget {
    const int buttonWidth = 44;
    const int buttonHeight = 15;
    const int buttonOffset = 3;
    std::shared_ptr<Font> font;

    TFormMenuRow();
};

struct TFormBankEditMainRow : TFormMenuRow {
    TFormEditorButton* loadButton;
    TFormEditorButton* copyButton;
    TFormEditorButton* pasteButton;
    TFormEditorButton* clearButton;
    TFormEditorButton* purgeButton;

    TFormEditorButton* viewButton;
    TFormEditorButton* backButton;
    int selectedBank;
    bool selectedBankIsFilled;

    TFormBankEditMainRow();
    void draw(const DrawArgs& args) override;
};

struct TFormClearRow : TFormMenuRow {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    int selectedBank;

    TFormClearRow();
    void draw(const DrawArgs& args) override;
};

struct TFormBankEditPurgeRow : TFormMenuRow {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    int selectedBank;

    TFormBankEditPurgeRow();
    void draw(const DrawArgs& args) override;
};

struct TFormBankEditCopyRow : TFormMenuRow {
    TFormEditorButton* cancelButton;
    TFormEditorButton* pasteButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;
    int destBank;

    TFormBankEditCopyRow();
    void draw(const DrawArgs& args) override;
};

struct TFormWaveViewPane : TFormMenuRow {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* backButton;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedBank = 0;
    int selectedWave = 0;

    float waveData[TFORM_MAX_NUM_WAVES][TFORM_MAX_WAVELENGTH];

    TFormWaveViewPane();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
};

#endif
