#include <iostream>
#include <unistd.h>

#include "core/Application.h"

int main() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Working directory: " << cwd << std::endl;
    }
    RendererAPI api = RendererAPI::OpenGL;
    Application app(api);
    app.Run();
}
