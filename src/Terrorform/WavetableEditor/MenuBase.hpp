#ifndef TFORM_EDITOR_MENU_ROWS
#define TFORM_EDITOR_MENU_ROWS
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"
#include "TFormEditorButton.hpp"
#include "TFormEditorChoice.hpp"
#include "TFormEditorGrid.hpp"
#include "WaveDisplay.hpp"

#define TFORM_EDITOR_COLS 8
#define TFORM_EDITOR_ROWS 8
#define TFORM_EDITOR_SLOTS (TFORM_EDITOR_ROWS * TFORM_EDITOR_COLS)

class TFormMenu : public OpaqueWidget {
public:
    const int buttonWidth = 44;
    const int buttonHeight = 15;
    const int buttonOffset = 3;

    std::function<void()> onView;
    std::function<void()> onHide;
    std::function<void()> onExit;

    TFormMenu();
    void view();
    void hide();

protected:
    void exit();
};

#endif
