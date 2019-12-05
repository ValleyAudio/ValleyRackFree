#include "WaveDisplay.hpp"

TFormEditorWaveDisplay::TFormEditorWaveDisplay() {
    offColor = nvgRGBA(0x00, 0x7F, 0x3F, 0x6F);
    onColor = nvgRGBA(0x00, 0xFF, 0x9F, 0xFF);
    bgColor = nvgRGBA(0x00, 0xFF, 0x9F, 0x4F);

    // offColor = nvgRGBA(0x70, 0x40, 0x00, 0x6F);
    // onColor = nvgRGBA(0xFF, 0xC0, 0x00, 0xFF);
    // bgColor = nvgRGBA(0xFF, 0xC0, 0x00, 0x4F);

    // offColor = nvgRGBA(0xAF, 0x00, 0x00, 0x6F);
    // onColor = nvgRGBA(0xFF, 0x00, 0x00, 0xFF);
    // bgColor = nvgRGBA(0xFF, 0x00, 0x00, 0x4F);

    // offColor = nvgRGBA(0x00, 0x4C, 0xDF, 0x6F);
    // onColor = nvgRGBA(0x00, 0xC0, 0xFF, 0xFF);
    // bgColor = nvgRGBA(0x00, 0xC0, 0xFF, 0x4F);

    selectedWave = 0;
    numWaves = 64;

    for (auto w = 0; w < TFORM_MAX_NUM_WAVES; ++w) {
        for (auto i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
            waveData[w][i] = 0.f;
        }
    }
}

void TFormEditorWaveDisplay::draw(const DrawArgs& args) {
    if (numWaves < 1) {
        return;
    }

    auto dimetricProject = [](float x, float y, float z) -> Vec {
        const float cos7 = 0.992546;
        const float cos42 = 0.743145;
        const float sin7 = 0.121869;
        const float sin42 = 0.669131;
        return Vec(x * cos7 + 0.5 * (z * cos42), y + 0.5 * (z * sin42) - x * sin7);
    };

    float dZ = (float) TFORM_MAX_WAVELENGTH / (float) TFORM_MAX_NUM_WAVES;
    const float xScale = (box.size.x - 10.f) / dimetricProject(255.f, 0.f, 64.f * dZ * 2.f).x;

    auto scalePoint = [=](const Vec& p, float xScale) -> Vec {
        Vec newP = p;
        newP.x *= xScale;
        newP.y *= 0.5;
        newP.y -= box.pos.y + box.size.y * 0.5f;
        return newP;
    };

    auto drawWaveLine = [=](int w, const NVGcolor& color) -> void {
        Vec pW;
        float z = (float) w * dZ * 2.f;

        nvgBeginPath(args.vg);
        nvgStrokeWidth(args.vg, 1.0);
        nvgStrokeColor(args.vg, color);
        nvgLineCap(args.vg, NVG_ROUND);
        nvgLineJoin(args.vg, NVG_ROUND);
        pW = dimetricProject(0, 0.f, z);
        pW = scalePoint(pW, xScale);

        nvgMoveTo(args.vg, pW.x + 6.f, -pW.y);
        for (int i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
            pW = dimetricProject((float) i, waveData[w][i] * 32.f, z);
            pW = scalePoint(pW, xScale);
            nvgLineTo(args.vg, pW.x + 6.f, -pW.y);
        }

        pW = dimetricProject(TFORM_MAX_WAVELENGTH, 0.f, z);
        pW = scalePoint(pW, xScale);
        nvgLineTo(args.vg, pW.x + 6.f, -pW.y);
        nvgStroke(args.vg);
        nvgLineCap(args.vg, NVG_BUTT);
        nvgLineJoin(args.vg, NVG_MITER);
    };

    auto drawWave = [=](int w, const NVGcolor& color) {
        Vec pW;
        float z = (float) w * dZ * 2.f;
        nvgBeginPath(args.vg);
        nvgFillColor(args.vg, color);
        pW = dimetricProject(0, 0.f, z);
        pW = scalePoint(pW, xScale);

        nvgMoveTo(args.vg, pW.x + 6.f, -pW.y);
        for (int i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
            pW = dimetricProject((float) i, waveData[w][i] * 32.f, z);
            pW = scalePoint(pW, xScale);
            nvgLineTo(args.vg, pW.x + 6.f, -pW.y);
        }

        pW = dimetricProject(TFORM_MAX_WAVELENGTH, 0.f, z);
        pW = scalePoint(pW, xScale);
        nvgLineTo(args.vg, pW.x + 6.f, -pW.y);
        nvgFill(args.vg);
    };

    drawWave(selectedWave, bgColor);
    for (int w = numWaves - 1; w >= 0; --w) {
        if (w == selectedWave) {
            continue;
        }
        drawWaveLine(w, offColor);
    }
    drawWaveLine(selectedWave, onColor);
    Widget::draw(args);
}
