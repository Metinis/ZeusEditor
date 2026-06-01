#include "EditorApp.h"
#include "src/config.h"

using namespace ZED;

EditorApp::EditorApp() {
    //first initialize needed paths and arguments from editor
    m_ResourceRoot = RESOURCE_ROOT;

    //can now actually initialize main app from engine
    init();

   
    pushLayer(new EditorLayer(&m_Ctx));
}


EditorApp::~EditorApp() = default;







