#include <iostream>
#include "core/Application.h"
#include "ZeusEngineCore/IRendererAPI.h"

int main() {
    ZEN::eRendererAPI api = ZEN::eRendererAPI::Vulkan;
    ZED::Application app(api);
    app.Run();
}
