#include "EditorApp.h"
#include "src/config.h"

using namespace ZED;

EditorApp::EditorApp(ZEN::eRendererAPI api) {
    //first initialize needed paths and arguments from editor
    m_ResourceRoot = RESOURCE_ROOT;
    m_API = api;

    //can now actually initialize main app from engine
    init();

    pushLayer(new EditorLayer(m_Engine.get()));
}


EditorApp::~EditorApp() = default;







