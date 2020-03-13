#ifndef TFORM_EDITOR_LOAD_MENU_HPP
#define TFORM_EDITOR_LOAD_MENU_HPP
#include <app/LedDisplay.hpp>
#include "MenuBase.hpp"

struct TFormTextField : TextField {
    std::shared_ptr<Font> font;
    NVGcolor color;
    NVGcolor textColor;

    TFormTextField();
    void draw(const DrawArgs& args) override;
    std::string getText() const;
};

struct TFormNumberField : TextField {
    std::shared_ptr<Font> font;
    NVGcolor color;
    NVGcolor textColor;

    int minimum;
    int maximum;
    int value;

    std::function<void()> onChangeCallback;

    TFormNumberField();
    void draw(const DrawArgs& args) override;
    void onDeselect(const event::Deselect& e) override;
    void onAction(const event::Action& e) override;
    void onDragMove(const event::DragMove& e) override;
    void setValue(int newValue);
private:
    void updateText(const std::string& newText);
    std::string prevText;
};

struct TFormLoadMenu : TFormMenu {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* cancelButton;
    TFormEditorButton* confirmButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    // TFormEditorNumberChoice* startWaveChoice;
    // TFormEditorNumberChoice* endWaveChoice;
    std::shared_ptr<std::vector<std::vector<float>>> detectedWaves;
    std::shared_ptr<int> selectedBank;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedWave;

    PlainText* nameFieldLabel;
    PlainText* startWaveFieldLabel;
    PlainText* endWaveFieldLabel;
    TFormTextField* nameField;
    TFormNumberField* startWaveField;
    TFormNumberField* endWaveField;

    std::function<void(int, int)> ingestNewTable;
    std::function<void(int, int, int, const std::string&)> onIngestTableCallback;

    TFormLoadMenu();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
};

#endif
