
#include "Editor.h"


Editor::Editor(ZEN::ZEngine* engine) : m_Engine(engine){
    m_MenuBarPanel = std::make_unique<MenuBarPanel>(m_Engine);
    m_InspectorPanel = std::make_unique<InspectorPanel>(m_Engine);
    m_ProjectPanel = std::make_unique<ProjectPanel>(m_Engine);
    m_ViewPanel = std::make_unique<ViewPanel>(m_Engine);
    m_ScenePanel = std::make_unique<ScenePanel>(m_Engine);
}

void Editor::onUpdate() {

}

void Editor::onUIRender() {
    m_MenuBarPanel->onImGuiRender();
    m_ScenePanel->onImGuiRender();
    m_ViewPanel->onImGuiRender();
    m_InspectorPanel->onImGuiRender();
    m_ProjectPanel->onImGuiRender();
}
