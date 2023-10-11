#ifndef TEXTY_CANVAS_H
#define TEXTY_CANVAS_H

#include <vector>

class TextyCanvas
{
public:
    void Create(int width, int height);
    void Clear(int colour);
    void Paint(int x, int y, int moveX, int moveY, int colour);
    int GetPixelColour(int x, int y);
    int GetWidth();
    int GetHeight();

private:
    int GetPixelIndex(int x, int y);

    std::vector<int> m_canvas;
    int m_width = 0;
    int m_height = 0;
};

#endif