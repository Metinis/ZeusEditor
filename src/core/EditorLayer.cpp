#include "EditorLayer.h"
#include "InspectorPanel.h"
#include "MenuBarPanel.h"
#include "ProjectPanel.h"
#include "ScenePanel.h"
#include "ViewPanel.h"

EditorLayer::EditorLayer(ZEN::ZEngine* engine) {
    m_InspectorPanel = new InspectorPanel(engine, m_SelectionContext);
    m_ProjectPanel = new ProjectPanel(engine, m_SelectionContext);
    m_MenuBarPanel = new MenuBarPanel(engine, m_SelectionContext);
    m_ScenePanel = new ScenePanel(engine, m_SelectionContext);
    m_ViewPanel = new ViewPanel(engine, m_SelectionContext);

    //pass their lifetime to layer stack
    ZEN::Application::get().pushOverlay(m_InspectorPanel);
    ZEN::Application::get().pushOverlay(m_ProjectPanel);
    ZEN::Application::get().pushOverlay(m_MenuBarPanel);
    ZEN::Application::get().pushOverlay(m_ScenePanel);
    ZEN::Application::get().pushOverlay(m_ViewPanel);
}

void EditorLayer::onEvent(ZEN::Event &event) {
    ZEN::EventDispatcher dispatcher(event);

    dispatcher.dispatch<ZEN::RunPlayModeEvent>([this](ZEN::RunPlayModeEvent& e) {return onRunPlayMode(e); });
}

bool EditorLayer::onRunPlayMode(ZEN::RunPlayModeEvent &e) {
    if(!e.getPlaying()) {
        ZEN::Application::get().pushOverlay(m_InspectorPanel);
        ZEN::Application::get().pushOverlay(m_ProjectPanel);
        ZEN::Application::get().pushOverlay(m_ScenePanel);
        ZEN::Application::get().pushOverlay(m_ViewPanel);
    }
    return false;
}

EditorLayer::~EditorLayer() = default;
