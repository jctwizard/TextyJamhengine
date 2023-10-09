#ifndef TEXTY_H
#define TEXTY_H

#include <jamhengine.h>

using namespace jhe;

class TextyApplication : public jhe::Application
{
public:
    TextyApplication(const char* applicationName);

    bool Init() override;
    void Run();

private:
    PixelRenderer renderer;
    glm::vec2 pixelPosition = glm::vec2(32.0f, 32.0f);
};

Application* CreateApplication() { return new TextyApplication("Texty!"); }

#endif