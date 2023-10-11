#ifndef TEXTY_H
#define TEXTY_H

#include "TextyCanvas.h"

#include <jamhengine.h>
#include <iostream>
#include <fstream>
#include <shlobj.h>

using namespace jhe;

class TextyApplication : public jhe::Application
{
public:
    TextyApplication(const char* applicationName);

    bool Init() override;
    void Run();

    void DrawCanvas(TextyCanvas canvas);

    std::string GetSavePath();
    glm::vec2 GetMousePixel();

private:
    const int SCREEN_WIDTH = 512;
    const int SCREEN_HEIGHT = 256;

    const int GAME_WIDTH = 16;
    const int GAME_HEIGHT = 8;

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