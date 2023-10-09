#include "TextyApplication.h"

TextyApplication::TextyApplication(const char* applicationName) : Application(applicationName)
{
    m_defaultWindowWidth = 512;
    m_defaultWindowHeight = 512;
}

bool TextyApplication::Init()
{
    if (!Application::Init())
    {
        return false;
    }

    renderer.Create(64, 64);

    return true;
}

void TextyApplication::Run()
{
    m_window->UpdateEvents();

    renderer.Clear(0);

    pixelPosition.x += Time::deltaTime() * 20;

    if (pixelPosition.x > 64)
    {
        pixelPosition.y += 1;
        pixelPosition.x = 0;

        if (pixelPosition.y > 64)
        {
            pixelPosition.y = 0;
        }
    }

    renderer.SetPixel((int)pixelPosition.x, (int)pixelPosition.y, ICOL_GREEN);

    renderer.Render();

    m_window->UpdateBuffers();

    m_running = !m_window->IsClosing();

    if (Input::GetKeyDown(GLFW_KEY_ESCAPE))
    {
        m_running = false;
    }
}