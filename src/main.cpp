#include "core/EditorApp.h"
#include "ZeusEngineCore/core/API.h"

int main(int argc, char** argv) {
    ZED::EditorApp* app = new ZED::EditorApp();
    app->run();
    delete(app);
}
