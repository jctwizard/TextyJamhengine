#ifndef TEXTY_H
#define TEXTY_H

#include "TextyCanvas.h"

#include <jamhengine.h>
#include <iostream>
#include <fstream>
#include <shlobj.h>
#include <direct.h>
#include<map>

using namespace jhe;
using namespace std;

class TextyApplication : public jhe::Application
{
public:
    TextyApplication(const char* applicationName);

    bool Init() override;
    void Run();

    void DrawCanvas(TextyCanvas canvas, bool clear);
    void CreateText(string text, int x, int y, int spacing);
    void ClearText();
    vector<vector<int>> LoadGlyph(string fileName, bool checkCache);
    bool SaveGlyph(string fileName, vector<vector<int>> data);

    char GetCharFromFileName(std::string fileName);
    std::string GetFileNameFromChar(char character);
    std::string GetSaveExtension();
    std::string GetSavePath();
    glm::vec2 GetMousePixel();

private:
    const string DEFAULT_FILENAME = "drawing";

    const int PIXEL_SCALE_X = 8;
    const int PIXEL_SCALE_Y = 8;

    const int GAME_WIDTH = 128;
    const int GAME_HEIGHT = 128;
    const int CANVAS_WIDTH = 5;
    const int CANVAS_HEIGHT = 5;

    const iColor WINDOW_COLOUR = 0xaaaaaa;
    const iColor CLEAR_COLOUR = 0;

    const iColor PAINT_COLOURS[4] = { 0xffffff, ICOL_RED, ICOL_GREEN, ICOL_BLUE };
    const iColor MOUSE_COLOUR = 0xbbbbbb;

    PixelRenderer m_renderer;
    TextyCanvas m_paintCanvas;
    vector<TextyCanvas> m_textCanvases;

    int m_currentPaintColourIndex = 0;
    glm::vec2 m_previousMousePixel = glm::vec2();

    std::map<string, vector<vector<int>>> m_fontCache;
};

Application* CreateApplication() { return new TextyApplication("Texty!"); }

#endif