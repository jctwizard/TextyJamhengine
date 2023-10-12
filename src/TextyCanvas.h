#ifndef TEXTY_CANVAS_H
#define TEXTY_CANVAS_H

#include <vector>

class TextyCanvas
{
public:
    void Create(int x, int y, int width, int height);
    void Clear(int colour);
    void Paint(int x, int y, int colour);
    int GetPixelColour(int x, int y);
    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();
    std::vector<std::vector<int>> GetData();
    void SetData(std::vector<std::vector<int>> data);

private:
    std::vector<std::vector<int>> m_data;
    int m_width = 0;
    int m_height = 0;
    int m_x = 0;
    int m_y = 0;
};

#endif