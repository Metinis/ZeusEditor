#include <iostream>

#include "core/Application.h"

int main() {
    ZEN::RendererAPI api = ZEN::RendererAPI::Vulkan;
    ZED::Application app(api);
    app.Run();
}
