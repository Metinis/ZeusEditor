#include <iostream>

#include "core/Application.h"

int main() {
    ZEN::eRendererAPI api = ZEN::eRendererAPI::Vulkan;
    ZED::Application app(api);
    app.Run();
}
