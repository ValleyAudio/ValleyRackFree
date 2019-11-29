#pragma once
#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "TerrorformEditorWaveDisplay.hpp"

#define TRRFORM_EDITOR_COLS 8
#define TRRFORM_EDITOR_ROWS 8
#define TRRFORM_EDITOR_SLOTS TRRFORM_EDITOR_ROWS * TRRFORM_EDITOR_COLS

enum TFormEditorButtonModes {
    IDLE_MODE,
    HOVER_MODE,
    HIGHLIGHT_MODE,
    HIGHLIGHT_HOVER_MODE,
    NUM_BUTTON_MODES
};

struct TFormEditorButtonStyle {
    NVGcolor textColor;
	NVGcolor bgColor;
    NVGcolor borderColor;

    TFormEditorButtonStyle();
    TFormEditorButtonStyle(const NVGcolor& newTextColor,
                           const NVGcolor& newBgColor,
                           const NVGcolor& newBorderColor);
};

enum TFormEditorStyles {
    RED_EDITOR_STYLE,
    YELLOW_EDITOR_STYLE,
    GREEN_EDITOR_STYLE,
    BLUE_EDITOR_STYLE,
    WHITE_EDITOR_STYLE,
    NUM_EDITOR_STYLES
};

struct TFormEditorButton : public OpaqueWidget {
    std::string text;
    std::shared_ptr<Font> font;
    NVGcolor color;
	NVGcolor bgColor;
    NVGcolor borderColor;
    Vec textOffset;
    float fontSize;
    bool enabled;

    TFormEditorButtonStyle buttonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonModes mode;
    TFormEditorButtonModes modeOffset;

    std::function<void()> onClick;

    TFormEditorButton();
    void draw(const DrawArgs& args) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void onDragEnd(const event::DragEnd& e) override;
    void setHighlight(bool highlight);
};

TFormEditorButton* createNewMenuButton(const std::string& text,
                                       const std::function<void()>& onClickCallback,
                                       int x, int y, int width, int height);

struct TFormEditorGrid : TransparentWidget {
    NVGcolor color;
    TFormEditorStyles style;

    TFormEditorGrid();
    void draw(const DrawArgs& args) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

struct TFormEditorNumberItem : MenuItem {
    unsigned long _itemNumber;
    std::shared_ptr<unsigned long> choice;

    TFormEditorNumberItem(unsigned long itemNumber);
    void onAction(const event::Action &e) override;
    void step() override;
};

struct TFormEditorNumberChoice : ChoiceButton {
    std::shared_ptr<unsigned long> choice;
    unsigned long range;

    std::shared_ptr<Font> font;
    NVGcolor boxColor;
    NVGcolor arrowColor;
    NVGcolor textColor;
    NVGcolor boxOnEnterColor;
    NVGcolor boxOnLeaveColor;
    NVGcolor arrowOnEnterColor;
    NVGcolor arrowOnLeaveColor;

    TFormEditorNumberChoice();
    void onAction(const event::Action &e) override;
    void onEnter(const event::Enter &e) override;
    void onLeave(const event::Leave &e) override;
    void draw(const DrawArgs& args) override;
};

struct TFormBankEditMainRow : OpaqueWidget {
    TFormEditorButton* loadButton;
    TFormEditorButton* copyButton;
    TFormEditorButton* pasteButton;
    TFormEditorButton* clearButton;
    TFormEditorButton* purgeButton;
    TFormEditorButton* backButton;
    int selectedBank;
    bool selectedBankIsFilled;

    int buttonWidth = 44;
    int buttonHeight = 15;
    int buttonOffset = 3;

    std::shared_ptr<Font> font;

    TFormBankEditMainRow();
    void draw(const DrawArgs& args) override;
};

struct TFormBankEditLoadRow : OpaqueWidget {
    TFormEditorButton* cancelButton;
    TFormEditorButton* confirmButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;
    int detectedWaves;

    int buttonWidth = 44;
    int buttonHeight = 15;
    int buttonOffset = 3;

    std::function<void(int, int)> ingestNewTable;

    std::shared_ptr<Font> font;

    TFormBankEditLoadRow();
    void draw(const DrawArgs& args) override;
};

struct TFormBankEditClearRow : OpaqueWidget {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    int selectedBank;

    int buttonWidth = 44;
    int buttonHeight = 15;
    int buttonOffset = 3;

    std::shared_ptr<Font> font;

    TFormBankEditClearRow();
    void draw(const DrawArgs& args) override;
};

struct TFormBankEditPurgeRow : OpaqueWidget {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    int selectedBank;

    int buttonWidth = 44;
    int buttonHeight = 15;
    int buttonOffset = 3;

    std::shared_ptr<Font> font;

    TFormBankEditPurgeRow();
    void draw(const DrawArgs& args) override;
};

struct TFormBankEditCopyRow : OpaqueWidget {
    TFormEditorButton* cancelButton;
    TFormEditorButton* pasteButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;
    int destBank;

    int buttonWidth = 44;
    int buttonHeight = 15;
    int buttonOffset = 3;

    std::shared_ptr<Font> font;

    TFormBankEditCopyRow();
    void draw(const DrawArgs& args) override;
};

struct TFormEditorBankEditMenu : OpaqueWidget {
    enum State {
        SELECT_BANK_STATE = 0,
        LOAD_WAVE_STATE,
        CLEAR_BANK_STATE,
        PURGE_STATE
    };

    State state;
    TFormBankEditMainRow* mainButtonRow;
    TFormBankEditLoadRow* loadButtonRow;
    TFormBankEditClearRow* clearButtonRow;
    TFormBankEditPurgeRow* purgeButtonRow;
    int selectedBank;

    TFormEditorGrid* grid;
    TFormEditorButton* slotButton[TRRFORM_EDITOR_ROWS][TRRFORM_EDITOR_COLS];
    TFormEditorButtonStyle emptySlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle filledSlotButtonStyles[NUM_BUTTON_MODES];

    bool slotFilled[TRRFORM_EDITOR_SLOTS];

    std::function<int()> onLoadWAVCallback;
    std::function<void(int, int, int)> onIngestTableCallback;
    std::function<void(int)> onClearBankCallback;
    std::function<void(int)> onCloneBankCallback;

    std::shared_ptr<Font> font;

    TFormEditorBankEditMenu();
    void step() override;
    void setSlotFilledFlag(int slot, bool isFilled);
    void changeState(const State newState);
};

struct TFormEditorMainMenu : OpaqueWidget {
    TFormEditorButton* editButton;
    TFormEditorButton* exitButton;
    TFormEditorButton* importButton;
    TFormEditorButton* exportButton;
    PlainText* title;
    int selectedBank;

    TFormEditorMainMenu();
};

struct TFormEditor : OpaqueWidget {
    TFormEditorMainMenu* mainMenu;
    TFormEditorBankEditMenu* editMenu;
    TFormEditorWaveDisplay* waveDisplay;

    TFormEditor();
    void addOnExitCallback(const std::function<void()>& onExitCallback);
    void addLoadWAVCallback(const std::function<int()>& onLoadWAVCallback);
    void addIngestTableCallback(const std::function<void(int, int, int)>& onIngestTableCallback);
    void addClearBankCallback(const std::function<void(int)>& onClearBankCallback);
    void addCloneBankCallback(const std::function<void(int)>& onCloneBankCallback);

    void addImportCallback(const std::function<void()>& onImportWaveTableCallback);
    void addExportCallback(const std::function<void()>& onExportWaveTableCallback);

    void setSlotFilledFlag(int slot, bool isFilled);
};
