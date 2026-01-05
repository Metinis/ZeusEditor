#include "core/EditorApp.h"
#include "ZeusEngineCore/core/API.h"

int main(int argc, char** argv) {
    ZEN::eRendererAPI api = ZEN::eRendererAPI::OpenGL;
    ZED::EditorApp* app = new ZED::EditorApp(api);
    app->run();
    delete(app);
}
