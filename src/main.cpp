#include <iostream>
#include "core/Application.h"
#include "ZeusEngineCore/API.h"

int main() {
    ZEN::eRendererAPI api = ZEN::eRendererAPI::OpenGL;
    ZED::Application app(api);
    app.run();
}
