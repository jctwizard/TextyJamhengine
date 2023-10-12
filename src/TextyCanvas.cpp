#include "TextyCanvas.h"

void TextyCanvas::Create(int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;

    m_width = width;
    m_height = height;

    Clear(0);
}

void TextyCanvas::Clear(int colour)
{
    m_data.clear();

    for (int y = 0; y < m_height; y++)
    {
        m_data.push_back({});

        for (int x = 0; x < m_width; x++)
        {
            m_data[y].push_back(colour);
        }
    }
}

void TextyCanvas::Paint(int x, int y, int colour)
{
    m_data[y][x] = colour;
}

int TextyCanvas::GetPixelColour(int x, int y)
{
    return m_data[y][x];
}

int TextyCanvas::GetX()
{
    return m_x;
}

int TextyCanvas::GetY()
{
    return m_y;
}

int TextyCanvas::GetWidth()
{
    return m_width;
}

int TextyCanvas::GetHeight()
{
    return m_height;
}

std::vector<std::vector<int>> TextyCanvas::GetData()
{
    return m_data;
}

void TextyCanvas::SetData(std::vector<std::vector<int>> data)
{
    m_data = data;
}