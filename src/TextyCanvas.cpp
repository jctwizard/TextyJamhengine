#include "TextyCanvas.h"

void TextyCanvas::Create(int width, int height)
{
    m_width = width;
    m_height = height;

    Clear(-1);
}

void TextyCanvas::Clear(int colour)
{
    m_canvas.clear();

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            m_canvas.push_back(colour);
        }
    }
}

void TextyCanvas::Paint(int x, int y, int moveX, int moveY, int colour)
{
    int pixelIndex = GetPixelIndex(x, y);
    m_canvas[pixelIndex] = colour;
}

int TextyCanvas::GetPixelColour(int x, int y)
{
    int pixelIndex = GetPixelIndex(x, y);
    return m_canvas[pixelIndex];
}

int TextyCanvas::GetWidth()
{
    return m_width;
}

int TextyCanvas::GetHeight()
{
    return m_height;
}

int TextyCanvas::GetPixelIndex(int x, int y)
{
    return (y * m_width) + x;
}
