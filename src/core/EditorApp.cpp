#include "EditorApp.h"
#include <ZeusEngineCore/ZEngine.h>
#include "src/config.h"
#include "../imgui/panels/InspectorPanel.h"
#include "../imgui/panels/MenuBarPanel.h"
#include "../imgui/panels/ProjectPanel.h"
#include "../imgui/panels/ScenePanel.h"
#include "../imgui/panels/ViewPanel.h"


using namespace ZED;

EditorApp::EditorApp(ZEN::eRendererAPI api) {
    //first initialize needed paths and arguments from editor
    m_ResourceRoot = RESOURCE_ROOT;
    m_API = api;

    //can now actually initialize main app from engine
    init();

    pushOverlay(new InspectorPanel(m_Engine.get()));
    pushOverlay(new ProjectPanel(m_Engine.get()));
    pushOverlay(new MenuBarPanel(m_Engine.get()));
    pushOverlay(new ScenePanel(m_Engine.get()));
    pushOverlay(new ViewPanel(m_Engine.get()));
}
EditorApp::~EditorApp() = default;







