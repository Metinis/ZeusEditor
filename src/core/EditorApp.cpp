#include "EditorApp.h"
#include "src/config.h"

using namespace ZED;

EditorApp::EditorApp() {
    init();
   
    pushLayer(new EditorLayer(&m_Ctx));
}


EditorApp::~EditorApp() = default;







