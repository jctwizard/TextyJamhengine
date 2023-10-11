#include "TextyApplication.h"

TextyApplication::TextyApplication(const char* applicationName) : Application(applicationName)
{
    m_defaultWindowWidth = SCREEN_WIDTH;
    m_defaultWindowHeight = SCREEN_HEIGHT;
}

bool TextyApplication::Init()
{
    if (!Application::Init())
    {
        return false;
    }

    m_renderer.Create(GAME_WIDTH, GAME_HEIGHT);
    m_renderer.Clear(CLEAR_COLOUR);

    m_canvas.Create(GAME_WIDTH, GAME_HEIGHT);

    m_previousMousePixel = Input::GetMousePosition();

    return true;
}

std::string TextyApplication::GetSavePath()
{
    char savePath[MAX_PATH] = { 0 };

    SHGetSpecialFolderPath(NULL, savePath, CSIDL_DESKTOPDIRECTORY, FALSE);

    return std::string(savePath) + "/";
}

void TextyApplication::Run()
{
    m_window->UpdateEvents();

    glm::vec2 mousePixel = GetMousePixel();

    if (Input::GetKeyDown(GLFW_KEY_SPACE))
    {
        m_canvas.Clear(-1);
    }

    if (Input::GetKeyDown(GLFW_KEY_W))
    {
        std::ofstream outputFile;
        outputFile.open(GetSavePath() + "drawing.txt");

        for (int pixelY = 0; pixelY < GAME_HEIGHT; pixelY++)
        {
            for (int pixelX = 0; pixelX < GAME_WIDTH; pixelX++)
            {
                outputFile << m_canvas.GetPixelColour(pixelX, pixelY) + 1;
            }
            outputFile << "\n";
        }

        outputFile.close();
    }

    if (Input::GetKeyDown(GLFW_KEY_R))
    {
        std::ifstream intputFile;
        intputFile.open(GetSavePath() + "drawing.txt");
        std::string nextLine = "";
        int y = 0;

        while (std::getline(intputFile, nextLine))
        {
            for (int x = 0; x < nextLine.length(); x++)
            {
                if (nextLine.at(x) != '\n')
                {
                    int pixelColour = (int)nextLine[x] - 48;

                    m_canvas.Paint(x, y, 1, 1, pixelColour - 1);
                }
            }

            y += 1;
        }

        intputFile.close();
    }

    if (Input::GetKeyDown(GLFW_KEY_LEFT))
    {
        // Set new mouse pixel
        m_currentPaintColourIndex -= 1;

        if (m_currentPaintColourIndex < 0)
        {
            m_currentPaintColourIndex = (sizeof(PAINT_COLOURS) / sizeof(iColor)) - 1;
        }
    }

    if (Input::GetKeyDown(GLFW_KEY_RIGHT))
    {
        // Set new mouse pixel
        m_currentPaintColourIndex += 1;

        if (m_currentPaintColourIndex >= sizeof(PAINT_COLOURS) / sizeof(iColor))
        {
            m_currentPaintColourIndex = 0;
        }
    }

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_1))
    {
        // Set new mouse pixel
        glm::vec2 mouseMove = mousePixel - m_previousMousePixel;

        if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_1))
        {
            mouseMove = glm::vec2(1, 1);
        }

        m_canvas.Paint(mousePixel.x, mousePixel.y, mouseMove.x, mouseMove.y, m_currentPaintColourIndex);
    }

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_2))
    {
        glm::vec2 mouseMove = mousePixel - m_previousMousePixel;

        if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_2))
        {
            mouseMove = glm::vec2(1, 1);
        }

        m_canvas.Paint(mousePixel.x, mousePixel.y, mouseMove.x, mouseMove.y, -1);
    }
        
    m_renderer.Clear(CLEAR_COLOUR);

    DrawCanvas(m_canvas);

    m_renderer.SetPixel(mousePixel.x, mousePixel.y, PAINT_COLOURS[m_currentPaintColourIndex]);

    m_renderer.Render();

    m_window->UpdateBuffers();

    m_running = !m_window->IsClosing();

    if (Input::GetKeyDown(GLFW_KEY_ESCAPE))
    {
        m_running = false;
    }

    m_previousMousePixel = mousePixel;
}

void TextyApplication::DrawCanvas(TextyCanvas canvas)
{
    for (int y = 0; y < canvas.GetHeight(); y++)
    {
        for (int x = 0; x < canvas.GetWidth(); x++)
        {
            int pixelColour = canvas.GetPixelColour(x, y);

            if (pixelColour == -1)
            {
                m_renderer.SetPixel(x, y, CLEAR_COLOUR);
            }
            else if (pixelColour >= 0 && pixelColour < sizeof(PAINT_COLOURS) / sizeof(iColor))
            {
                m_renderer.SetPixel(x, y, PAINT_COLOURS[pixelColour]);
            }
        }
    }
}

glm::vec2 TextyApplication::GetMousePixel()
{
    float pixelRatioX = (float)GAME_WIDTH / (float)SCREEN_WIDTH;
    float pixelRatioY = (float)GAME_HEIGHT / (float)SCREEN_HEIGHT;

    int mousePixelX = clamp((int)floor(Input::GetMousePosition().x * pixelRatioX), 0, GAME_WIDTH - 1);
    int mousePixelY = clamp((int)floor(Input::GetMousePosition().y * pixelRatioY), 0, GAME_HEIGHT - 1);
    
    return glm::vec2(mousePixelX, mousePixelY);
}