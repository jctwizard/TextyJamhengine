#include "TextyApplication.h"

TextyApplication::TextyApplication(const char* applicationName) : Application(applicationName)
{
    m_defaultWindowWidth = GAME_WIDTH * PIXEL_SCALE_X;
    m_defaultWindowHeight = GAME_HEIGHT * PIXEL_SCALE_Y;
}

bool TextyApplication::Init()
{
    if (!Application::Init())
    {
        return false;
    }

    m_renderer.Create(GAME_WIDTH, GAME_HEIGHT);
    m_renderer.Clear(CLEAR_COLOUR);

    m_canvas.Create(CANVAS_WIDTH, CANVAS_HEIGHT);

    m_previousMousePixel = Input::GetMousePosition();

    return true;
}

string TextyApplication::GetSavePath()
{
    char savePath[MAX_PATH] = { 0 };

    SHGetSpecialFolderPath(NULL, savePath, CSIDL_DESKTOPDIRECTORY, FALSE);

    return string(savePath) + "/";
}

string TextyApplication::GetSaveExtension()
{
    return ".txt";
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
        string fileName;

        // Take input using cin 
        cin >> fileName;

        if (fileName == "")
        {
            fileName = DEFAULT_FILENAME;
        }

        ofstream outputFile;
        outputFile.open(GetSavePath() + fileName + GetSaveExtension());

        for (int pixelY = 0; pixelY < m_canvas.GetHeight(); pixelY++)
        {
            for (int pixelX = 0; pixelX < m_canvas.GetWidth(); pixelX++)
            {
                outputFile << m_canvas.GetPixelColour(pixelX, pixelY) + 1;
            }
            outputFile << "\n";
        }

        cout << "saved file to Desktop: " + fileName + GetSaveExtension();

        outputFile.close();
    }

    if (Input::GetKeyDown(GLFW_KEY_R))
    {
        string fileName;

        // Take input using cin 
        cin >> fileName;

        ifstream inputFile;
        inputFile.open(GetSavePath() + fileName + GetSaveExtension());

        if (inputFile.is_open())
        {
            string nextLine = "";
            int y = 0;

            while (getline(inputFile, nextLine))
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

            inputFile.close();
        }
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
    
    m_renderer.SetPixel(mousePixel.x, mousePixel.y, PAINT_COLOURS[m_currentPaintColourIndex], true);

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
    int mousePixelX = clamp((int)floor(Input::GetMousePosition().x / (float)PIXEL_SCALE_X), 0, GAME_WIDTH - 1);
    int mousePixelY = clamp((int)floor(Input::GetMousePosition().y / (float)PIXEL_SCALE_Y), 0, GAME_HEIGHT - 1);
    
    return glm::vec2(mousePixelX, mousePixelY);
}