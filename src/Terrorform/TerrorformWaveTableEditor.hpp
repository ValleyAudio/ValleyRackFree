#pragma once
#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "WavetableEditor/MenuRows.hpp"
#include "WavetableEditor/TFormEditorButton.hpp"
#include "WavetableEditor/WaveLoader.hpp"
#include "WavetableEditor/TFormEditorGrid.hpp"

struct TFormEditorBankEditMenu : OpaqueWidget {
    enum State {
        SELECT_BANK_STATE = 0,
        LOAD_WAVE_STATE,
        VIEW_BANK_STATE,
        CLONE_BANK_STATE,
        CLEAR_BANK_STATE,
        PURGE_STATE
    };

    State state;
    TFormBankEditMainRow* mainButtonRow;
    TFormBankEditLoadRow* loadButtonRow;
    TFormWaveViewPane* viewPane;
    TFormCloneRow* cloneMenu;
    TFormClearRow* clearButtonRow;
    TFormPurgeRow* purgeButtonRow;

    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> selectedBank;
    std::shared_ptr<bool> selectedBankIsFilled;

    std::function<std::shared_ptr<std::vector<std::vector<float>>>()> onLoadWAVCallback;
    std::function<void(int, int, int)> onIngestTableCallback;
    std::function<void(int)> onClearBankCallback;
    std::function<void(int, std::vector<std::vector<float>>&)> onViewBankCallback;

    std::shared_ptr<Font> font;

    TFormEditorBankEditMenu();
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
    void addCloneBankCallback(const std::function<void(int, int)>& onCloneBankCallback);
    void addViewBankCallback(const std::function<void(int, std::vector<std::vector<float>>&)>& onViewBankCallback);

    void addImportCallback(const std::function<void()>& onImportWaveTableCallback);
    void addExportCallback(const std::function<void()>& onExportWaveTableCallback);

    void setSlotFilledFlag(int slot, bool isFilled);
};
