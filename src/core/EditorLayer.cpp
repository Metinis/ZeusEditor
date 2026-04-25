#include "EditorLayer.h"
#include "../imgui/panels/InspectorPanel.h"
#include "../imgui/panels/MenuBarPanel.h"
#include "../imgui/panels/ProjectPanel.h"
#include "../imgui/panels/ScenePanel.h"
#include "../imgui/panels/ViewPanel.h"

EditorLayer::EditorLayer(ZEN::EngineContext* ctx) {

    ImGui::SetCurrentContext(ZEN::getEngineImGuiContext());

    m_InspectorPanel = new InspectorPanel(ctx, m_SelectionContext);
    m_ProjectPanel = new ProjectPanel(ctx, m_SelectionContext);
    m_MenuBarPanel = new MenuBarPanel(ctx, m_SelectionContext);
    m_ScenePanel = new ScenePanel(ctx, m_SelectionContext);
    m_ViewPanel = new ViewPanel(ctx, m_SelectionContext);

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
