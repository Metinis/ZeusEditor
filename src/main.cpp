#include <iostream>

#include "core/Application.h"

int main() {
    RendererAPI api = RendererAPI::Vulkan;
    Application app(api);
    app.Run();
}
