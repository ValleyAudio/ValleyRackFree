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

class TFormMenuRow : public OpaqueWidget {
public:
    const int buttonWidth = 44;
    const int buttonHeight = 15;
    const int buttonOffset = 3;
    std::shared_ptr<Font> font;

    std::function<void()> onView;
    std::function<void()> onHide;
    std::function<void()> onExit;

    TFormMenuRow();
    void view();
    void hide();

protected:
    void exit();
};

#endif
