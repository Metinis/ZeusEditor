#include "EditorApp.h"
#include "src/config.h"
#include "InspectorPanel.h"
#include "MenuBarPanel.h"
#include "ProjectPanel.h"
#include "ScenePanel.h"
#include "ViewPanel.h"


using namespace ZED;

EditorApp::EditorApp(ZEN::eRendererAPI api) {
    //first initialize needed paths and arguments from editor
    m_ResourceRoot = RESOURCE_ROOT;
    m_API = api;

    //can now actually initialize main app from engine
    init();

    pushOverlay(new InspectorPanel(m_Engine.get(), m_SelectionContext));
    pushOverlay(new ProjectPanel(m_Engine.get(), m_SelectionContext));
    pushOverlay(new MenuBarPanel(m_Engine.get(), m_SelectionContext));
    pushOverlay(new ScenePanel(m_Engine.get(), m_SelectionContext));
    pushOverlay(new ViewPanel(m_Engine.get(), m_SelectionContext));
}


EditorApp::~EditorApp() = default;







