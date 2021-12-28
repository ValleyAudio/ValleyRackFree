#pragma once
#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "../Common/Utilities.hpp"
#include "WavetableEditor/MenuBase.hpp"
#include "WavetableEditor/TFormEditorButton.hpp"
#include "WavetableEditor/TFormEditorGrid.hpp"
#include "WavetableEditor/CloneMenu.hpp"
#include "WavetableEditor/MoveMenu.hpp"
#include "WavetableEditor/ClearMenu.hpp"
#include "WavetableEditor/ViewPane.hpp"
#include "WavetableEditor/LoadMenu.hpp"
#include "WavetableEditor/MainMenu.hpp"
#include "TerrorformWaveBank.hpp"

struct TFormEditorBankEditMenu : OpaqueWidget {
    TFormEditMainMenu* mainMenu;
    TFormLoadMenu* loadMenu;
    TFormWaveViewPane* viewPane;
    TFormCloneMenu* cloneMenu;
    TFormMoveMenu* moveMenu;
    TFormClearMenu* clearMenu;
    std::string dir;
    std::shared_ptr<int> selectedBank;
    std::vector<bool> slotFilled;

    std::function<std::shared_ptr<std::vector<float>>(char* path)> onLoadWAVCallback;
    std::function<void(int, TerrorformWaveBank&)> onGetBankCallback;

    TFormEditorBankEditMenu();
    void setSlotFilledFlag(int slot, bool isFilled);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct TFormEditorDefragMenu : TFormMenu {
    PlainText* defragDoneMessage;
    int viewCounter;

    TFormEditorDefragMenu();
    void step() override;
};

struct TFormPurgeMenu : TFormMenu {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    PlainText* questionText;
    PlainText* clearedText;
    int counter;

    std::function<void(int)> onClearBankCallback;

    TFormPurgeMenu();
    void step() override;
};

struct TFormEditorMainMenu : TFormMenu {
    TFormEditorButton* editButton;
    TFormEditorButton* exitButton;
    TFormEditorButton* importButton;
    TFormEditorButton* exportButton;
    TFormEditorButton* defragButton;
    TFormEditorButton* purgeButton;

    TFormEditorDefragMenu* defragMenu;
    TFormPurgeMenu* purgeMenu;

    PlainText* title;
    int selectedBank;

    std::function<void()> onDefragmentCallback;

    TFormEditorMainMenu();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct TFormEditor : OpaqueWidget {
    TFormEditorMainMenu* mainMenu;
    TFormEditorBankEditMenu* editMenu;

    TFormEditor();
    void addOnExitCallback(const std::function<void()>& onExitCallback);
    void addLoadWAVCallback(const std::function<std::shared_ptr<std::vector<float>>(char* path)>& onLoadWAVCallback);
    void addIngestTableCallback(const std::function<void(int, int, int, int, const std::string&)>& onIngestTableCallback);
    void addClearBankCallback(const std::function<void(int)>& onClearBankCallback);
    void addCloneBankCallback(const std::function<void(int, int, int, int)>& onCloneBankCallback);
    void addMoveBankCallback(const std::function<void(int, int)>& onMoveBankCallback);
    void addGetBankCallback(const std::function<void(int, TerrorformWaveBank&)>& onGetBankCallback);
    void addRenameBankCallback(const std::function<void(int, const std::string&)>& onRenameBankCallback);
    void addImportCallback(const std::function<void()>& onImportWaveTableCallback);
    void addExportCallback(const std::function<void()>& onExportWaveTableCallback);
    void addDefragmentCallback(const std::function<void()>& onDefragmentCallback);

    void setSlotFilledFlag(int slot, bool isFilled);
};
