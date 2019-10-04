#pragma once
#include "../Valley.hpp"
#include "../ValleyComponents.hpp"

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

struct TFormEditorGrid : TransparentWidget {
    NVGcolor color;
    TFormEditorStyles style;

    TFormEditorGrid();
    void draw(const DrawArgs& args) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

struct TFormEditorMainMenu : OpaqueWidget {
    TFormEditorButton* editButton;
    TFormEditorButton* exitButton;
    TFormEditorButton* importButton;
    TFormEditorButton* exportButton;

    TFormEditorMainMenu();
};

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


struct TFormEditorEditMenu : OpaqueWidget {
    enum TFormEditorEditMenuState {
        SELECT_BANK_STATE = 0,
        LOAD_WAVE_STATE,
        CLEAR_BANK_STATE
    };

    TFormEditorEditMenuState state;

    TFormEditorButton* loadButton;
    TFormEditorButton* cloneButton;
    TFormEditorButton* clearButton;
    TFormEditorButton* purgeButton;
    TFormEditorButton* backButton;

    TFormEditorButton* clearYesButton;
    TFormEditorButton* clearNoButton;

    TFormEditorButton* cancelButton;
    TFormEditorButton* confirmButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;

    TFormEditorGrid* grid;
    TFormEditorButton* slotButton[TRRFORM_EDITOR_ROWS][TRRFORM_EDITOR_COLS];
    TFormEditorButtonStyle emptySlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle filledSlotButtonStyles[NUM_BUTTON_MODES];

    int detectedWaves;
    std::string strDetectedWaves;
    int selectedBank;
    bool slotFilled[TRRFORM_EDITOR_SLOTS];

    std::function<int()> onLoadWAVCallback;
    std::function<void(int, int, int)> onIngestTableCallback;

    std::shared_ptr<Font> font;

    TFormEditorEditMenu();
    void step() override;
    void draw(const DrawArgs& args) override;
    void setSlotFilledFlag(int slot, bool isFilled);
};

struct TFormEditor : OpaqueWidget {
    TFormEditorMainMenu* mainMenu;
    TFormEditorEditMenu* editMenu;
    TFormEditorButton* exitButton;

    TFormEditor();
    void addOnExitCallback(const std::function<void()>& onExitCallback);
    void addLoadWAVCallback(const std::function<int()>& onLoadWAVCallback);
    void addIngestTableCallback(const std::function<void(int, int, int)>& onIngestTableCallback);
    void setStyle(const TFormEditorStyles& newStyle);
    void setSlotFilledFlag(int slot, bool isFilled);
};
