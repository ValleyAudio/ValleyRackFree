#pragma once
#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "WavetableEditor/MenuRows.hpp"
#include "WavetableEditor/TFormEditorButton.hpp"
#include "WavetableEditor/WaveLoader.hpp"
#include "WavetableEditor/TFormEditorGrid.hpp"

#define TRRFORM_EDITOR_COLS 8
#define TRRFORM_EDITOR_ROWS 8
#define TRRFORM_EDITOR_SLOTS TRRFORM_EDITOR_ROWS * TRRFORM_EDITOR_COLS

struct TFormEditorBankEditMenu : OpaqueWidget {
    enum State {
        SELECT_BANK_STATE = 0,
        LOAD_WAVE_STATE,
        CLEAR_BANK_STATE,
        VIEW_BANK_STATE,
        PURGE_STATE
    };

    State state;
    TFormBankEditMainRow* mainButtonRow;
    TFormBankEditLoadRow* loadButtonRow;
    TFormClearRow* clearButtonRow;
    TFormBankEditPurgeRow* purgeButtonRow;
    TFormWaveViewPane* viewPane;
    int selectedBank;

    TFormEditorGrid<TRRFORM_EDITOR_ROWS, TRRFORM_EDITOR_COLS>* grid;
    TFormEditorButtonStyle emptySlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle filledSlotButtonStyles[NUM_BUTTON_MODES];

    bool slotFilled[TRRFORM_EDITOR_SLOTS];

    std::function<std::shared_ptr<std::vector<std::vector<float>>>()> onLoadWAVCallback;
    std::function<void(int, int, int)> onIngestTableCallback;
    std::function<void(int)> onClearBankCallback;
    std::function<void(int)> onCloneBankCallback;
    std::function<void(int, float[TFORM_MAX_NUM_WAVES][TFORM_MAX_WAVELENGTH])> onViewBankCallback;

    std::shared_ptr<Font> font;

    TFormEditorBankEditMenu();
    void step() override;
    void setSlotFilledFlag(int slot, bool isFilled);
    void changeState(const State newState);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct TFormEditorMainMenu : OpaqueWidget {
    TFormEditorButton* editButton;
    TFormEditorButton* exitButton;
    TFormEditorButton* importButton;
    TFormEditorButton* exportButton;
    PlainText* title;
    int selectedBank;

    TFormEditorMainMenu();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct TFormEditor : OpaqueWidget {
    TFormEditorMainMenu* mainMenu;
    TFormEditorBankEditMenu* editMenu;

    TFormEditor();
    void addOnExitCallback(const std::function<void()>& onExitCallback);
    void addLoadWAVCallback(const std::function<std::shared_ptr<std::vector<std::vector<float>>>()>& onLoadWAVCallback);
    void addIngestTableCallback(const std::function<void(int, int, int)>& onIngestTableCallback);
    void addClearBankCallback(const std::function<void(int)>& onClearBankCallback);
    void addCloneBankCallback(const std::function<void(int)>& onCloneBankCallback);
    void addViewBankCallback(const std::function<void(int, float[TFORM_MAX_NUM_WAVES][TFORM_MAX_WAVELENGTH])>& onViewBankCallback);

    void addImportCallback(const std::function<void()>& onImportWaveTableCallback);
    void addExportCallback(const std::function<void()>& onExportWaveTableCallback);

    void setSlotFilledFlag(int slot, bool isFilled);
};
