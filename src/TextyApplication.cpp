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

    m_paintCanvas.Create(1, 1, CANVAS_WIDTH, CANVAS_HEIGHT);

    m_previousMousePixel = Input::GetMousePosition();

    return true;
}

string TextyApplication::GetSavePath()
{
    char savePath[MAX_PATH] = { 0 };

    SHGetSpecialFolderPath(NULL, savePath, CSIDL_DESKTOPDIRECTORY, FALSE);

    return string(savePath) + "/Texty/";
}

const char validChars[] = { 'a', 'z', 'A', 'Z', '0', '9' };

string TextyApplication::GetFileNameFromChar(char character)
{
    string fileName = " ";
    fileName[0] = character;

    bool valid = false;

    for (int limit = 0; limit < (sizeof(validChars) / sizeof(int)); limit += 2)
    {
        if ((int)character >= (int)validChars[limit] && (int)character <= (int)validChars[limit + 1])
        {
            valid = true;
            break;
        }
    }

    // if the character is not a valid filename then use the character code with prefix "c"
    if (valid == false)
    {
        fileName = "c" + to_string((int)character);
    }

    return fileName;
}

char TextyApplication::GetCharFromFileName(string fileName)
{
    if (fileName.size() == 1)
    {
        return fileName[0];
    }
    else if (fileName.size() > 1 && fileName[0] == 'c')
    {
        string characterCodeString = fileName.substr(1, fileName.size() - 1);

        // the rest of the filename is digits
        if (characterCodeString.find_first_not_of("0123456789") == string::npos)
        {
            return (char)stoi(characterCodeString);
        }
    }

    return ' ';
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
        m_paintCanvas.Clear(0);
    }

    if (Input::GetKeyDown(GLFW_KEY_F))
    {
        ClearText();
        CreateText("ABCDEFGHIJ", 1, 10, 1);
        CreateText("KLMNOPQRST", 1, 20, 1);
        CreateText("UVQXYZ?/|\\", 1, 30, 1);
        CreateText("abcdefghij", 1, 40, 1);
        CreateText("klmnopqrst", 1, 50, 1);
        CreateText("uvwxyz0123", 1, 60, 1);
        CreateText("456789!\"£$", 1, 70, 1);
        CreateText("%^&*()-_=+", 1, 80, 1);
        CreateText("{}[]:;@'~#<", 1, 90, 1);
        CreateText(",>.", 1, 100, 1);
    }

    if (Input::GetKeyDown(GLFW_KEY_W))
    {
        string fileName;

        cout << "\n\npreparing to write file, enter name (no extension)\n";

        _mkdir(GetSavePath().c_str());

        // Take input using cin 
        cin >> fileName;

        if (fileName == "")
        {
            fileName = DEFAULT_FILENAME;
        }

        bool success = SaveGlyph(fileName, m_paintCanvas.GetData());

        if (success)
        {
            cout << "\nsuccessfully saved file to Desktop/Texty: " + fileName + GetSaveExtension() + "\n";
        }
        else
        {
            cout << "\nfailed to save file to Desktop/Texty: " + fileName + GetSaveExtension() + "\n";
        }
    }

    if (Input::GetKeyDown(GLFW_KEY_R))
    {
        string fileName;

        cout << "\n\npreparing to read file, enter name (no extension)\n";

        // Take input using cin 
        cin >> fileName;

        if (fileName == "")
        {
            fileName = DEFAULT_FILENAME;
        }

        vector<vector<int>> data = LoadGlyph(fileName, false);

        if (data.size() > 0)
        {
            m_paintCanvas.SetData(data);

            cout << "\nsuccessfully read file from Desktop/Texty: " + fileName + GetSaveExtension();
        }
        else
        {
            cout << "\nfailed to read file from Desktop/Texty: " + fileName + GetSaveExtension();
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

    if (mousePixel.x >= m_paintCanvas.GetX() && mousePixel.x < m_paintCanvas.GetX() + m_paintCanvas.GetWidth() && mousePixel.y >= m_paintCanvas.GetY() && mousePixel.y < m_paintCanvas.GetHeight() + m_paintCanvas.GetY())
    {
        if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_1))
        {
            m_paintCanvas.Paint(mousePixel.x - m_paintCanvas.GetX(), mousePixel.y - m_paintCanvas.GetY(), m_currentPaintColourIndex + 1);
        }

        if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_2))
        {
            m_paintCanvas.Paint(mousePixel.x - m_paintCanvas.GetX(), mousePixel.y - m_paintCanvas.GetY(), 0);
        }
    }
        
    m_renderer.Clear(WINDOW_COLOUR);

    DrawCanvas(m_paintCanvas, true);

    for (int textCanvas = 0; textCanvas < m_textCanvases.size(); textCanvas++)
    {
        DrawCanvas(m_textCanvases[textCanvas], false);
    }
    
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

void TextyApplication::DrawCanvas(TextyCanvas canvas, bool clear)
{
    for (int y = 0; y < canvas.GetHeight(); y++)
    {
        for (int x = 0; x < canvas.GetWidth(); x++)
        {
            int pixelColour = canvas.GetPixelColour(x, y) - 1;

            if (canvas.GetX() + x >= 0 && canvas.GetX() + x < GAME_WIDTH && canvas.GetY() + y >= 0 && canvas.GetY() + y < GAME_HEIGHT)
            {
                if (pixelColour == -1 && clear)
                {
                    m_renderer.SetPixel(canvas.GetX() + x, canvas.GetY() + y, CLEAR_COLOUR);
                }
                else if (pixelColour >= 0 && pixelColour < sizeof(PAINT_COLOURS) / sizeof(iColor))
                {
                    m_renderer.SetPixel(canvas.GetX() + x, canvas.GetY() + y, PAINT_COLOURS[pixelColour]);
                }
            }
        }
    }
}

vector<vector<int>> TextyApplication::LoadGlyph(string fileName, bool checkCache)
{
    if (checkCache)
    {
        if (m_fontCache.find(fileName) != m_fontCache.end())
        {
            return m_fontCache[fileName];
        }
    }

    ifstream inputFile;

    string validFileName = fileName;

    if (validFileName.size() == 1)
    {
        validFileName = GetFileNameFromChar(validFileName[0]);
    }

    inputFile.open(GetSavePath() + validFileName + GetSaveExtension());

    if (inputFile.is_open())
    {
        string nextLine = "";
        int y = 0;

        m_paintCanvas.Clear(0);

        vector<vector<int>> data;

        while (getline(inputFile, nextLine))
        {
            data.push_back({});

            for (int x = 0; x < nextLine.length(); x++)
            {
                if (nextLine.at(x) != '\n')
                {
                    int pixelColour = (int)nextLine[x] - 48;

                    data[y].push_back(pixelColour);
                }
            }

            y += 1;
        }

        if (data.size() > 0 && fileName.length() == 1)
        {
            m_fontCache[fileName] = data;
        }

        inputFile.close();

        return data;
    }
    else
    {
        return {};
    }
}

bool TextyApplication::SaveGlyph(string fileName, vector<vector<int>> data)
{
    if (data.size() > 0 && fileName.length() == 1)
    {
        m_fontCache[fileName] = data;
    }

    ofstream outputFile;

    string validFileName = fileName;

    if (validFileName.size() == 1)
    {
        validFileName = GetFileNameFromChar(validFileName[0]);
    }

    outputFile.open(GetSavePath() + validFileName + GetSaveExtension());

    if (outputFile.is_open())
    {
        for (int pixelY = 0; pixelY < m_paintCanvas.GetHeight(); pixelY++)
        {
            for (int pixelX = 0; pixelX < m_paintCanvas.GetWidth(); pixelX++)
            {
                outputFile << data[pixelY][pixelX];
            }
            outputFile << "\n";
        }

        outputFile.close();

        return true;
    }
    else
    {
        return false;
    }
}

void TextyApplication::CreateText(std::string text, int x, int y, int spacing)
{
    int width = 0;

    for (int glyph = 0; glyph < text.length(); glyph++)
    {
        vector<vector<int>> glyphData = LoadGlyph(text.substr(glyph, 1), true);

        if (glyphData.size() > 0 && glyphData[0].size() > 0)
        {
            TextyCanvas newCanvas;
            newCanvas.Create(x + width + spacing * glyph, y, glyphData[0].size(), glyphData.size());
            newCanvas.SetData(glyphData);
            m_textCanvases.push_back(newCanvas);

            width += glyphData[0].size();
        }
    }
}

void TextyApplication::ClearText()
{
    m_textCanvases.clear();
}

glm::vec2 TextyApplication::GetMousePixel()
{
    int mousePixelX = clamp((int)floor(Input::GetMousePosition().x / (float)PIXEL_SCALE_X), 0, GAME_WIDTH - 1);
    int mousePixelY = clamp((int)floor(Input::GetMousePosition().y / (float)PIXEL_SCALE_Y), 0, GAME_HEIGHT - 1);
    
    return glm::vec2(mousePixelX, mousePixelY);
}