#ifndef TEXTY_H
#define TEXTY_H

#include "TextyCanvas.h"

#include <jamhengine.h>
#include <iostream>
#include <fstream>
#include <shlobj.h>

using namespace jhe;
using namespace std;

class TextyApplication : public jhe::Application
{
public:
    TextyApplication(const char* applicationName);

    bool Init() override;
    void Run();

    void DrawCanvas(TextyCanvas canvas);

    std::string GetSaveExtension();
    std::string GetSavePath();
    glm::vec2 GetMousePixel();

private:
    const string DEFAULT_FILENAME = "drawing";

    const int PIXEL_SCALE_X = 16;
    const int PIXEL_SCALE_Y = 16;

    const int GAME_WIDTH = 32;
    const int GAME_HEIGHT = 32;
    const int CANVAS_WIDTH = 5;
    const int CANVAS_HEIGHT = 5;

    const iColor CLEAR_COLOUR = 0;

    const iColor PAINT_COLOURS[4] = { 0xffffff, ICOL_RED, ICOL_GREEN, ICOL_BLUE };
    const iColor MOUSE_COLOUR = 0xbbbbbb;

    PixelRenderer m_renderer;
    TextyCanvas m_canvas;

    int m_currentPaintColourIndex = 0;
    glm::vec2 m_previousMousePixel = glm::vec2();
};

Application* CreateApplication() { return new TextyApplication("Texty!"); }

#endif