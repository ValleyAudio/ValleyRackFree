#ifndef TFORM_EDITOR_MENU_ROWS
#define TFORM_EDITOR_MENU_ROWS
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"
#include "TFormEditorButton.hpp"
#include "TFormEditorNumberChoice.hpp"
#include "TFormEditorGrid.hpp"
#include "WaveDisplay.hpp"

#define TFORM_EDITOR_COLS 8
#define TFORM_EDITOR_ROWS 8
#define TFORM_EDITOR_SLOTS TFORM_EDITOR_ROWS * TFORM_EDITOR_COLS

struct TFormMenuRow : OpaqueWidget {
    const int buttonWidth = 44;
    const int buttonHeight = 15;
    const int buttonOffset = 3;
    std::shared_ptr<Font> font;

    std::function<void()> onView;
    std::function<void()> onHide;

    TFormMenuRow();
    void view();
    void hide();
};

struct TFormBankEditMainRow : TFormMenuRow {
    TFormEditorButton* loadButton;
    TFormEditorButton* viewButton;
    TFormEditorButton* cloneButton;
    TFormEditorButton* clearButton;
    TFormEditorButton* purgeButton;
    TFormEditorButton* backButton;
    TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>* grid;
    TFormEditorButtonStyle emptySlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle filledSlotButtonStyles[NUM_BUTTON_MODES];

    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> selectedBank;
    bool selectedBankIsFilled;

    TFormBankEditMainRow();
    void step() override;
    void draw(const DrawArgs& args) override;
    void setSlotFilledFlag(int slot, bool isFilled);
};

struct TFormBankEditLoadRow : TFormMenuRow {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* cancelButton;
    TFormEditorButton* confirmButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;
    std::shared_ptr<std::vector<std::vector<float>>> detectedWaves;
    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> selectedBank;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedWave;

    std::function<void(int, int)> ingestNewTable;
    std::function<void(int, int, int)> onIngestTableCallback;

    TFormBankEditLoadRow();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
};

struct TFormClearRow : TFormMenuRow {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    std::shared_ptr<int> selectedBank;

    std::function<void(int)> onClearBankCallback;

    TFormClearRow();
    void draw(const DrawArgs& args) override;
};

struct TFormPurgeRow : TFormMenuRow {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;

    std::function<void(int)> onClearBankCallback;

    TFormPurgeRow();
    void draw(const DrawArgs& args) override;
};

struct TFormCloneRow : TFormMenuRow {
    enum TFormCloneRowState {
        SELECT_SOURCE_STATE,
        SELECT_DEST_STATE,
        CONFIRM_OVERWRITE_STATE
    };
    TFormCloneRowState state;

    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* selectSourceBackButton;
    TFormEditorButton* selectDestBackButton;
    TFormEditorButton* nextButton;
    TFormEditorButton* okayButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;
    TFormEditorButtonStyle emptySlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle sourceSlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle destSlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle filledSlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>* grid;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedWave;

    std::vector<std::vector<float>> waveData;
    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> sourceBank;
    int destBank;

    std::function<void(int, int)> onCloneBankCallback;

    TFormCloneRow();
    void step() override;
    void draw(const DrawArgs& args) override;
    void displaySourceSelection(const DrawArgs& args);
    void displayDestinationSelection(const DrawArgs& args);
    void changeState(TFormCloneRowState newState);

    void onDragMove(const event::DragMove& e) override;
    void setSlotFilledFlag(int slot, bool isFilled);
};

struct TFormWaveViewPane : TFormMenuRow {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* backButton;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    std::shared_ptr<int> selectedBank;
    int selectedWave = 0;

    std::vector<std::vector<float>> waveData;

    TFormWaveViewPane();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
};

#endif
