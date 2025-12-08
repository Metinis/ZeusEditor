#pragma once
#include <ZeusEngine.h>

#include "SelectionContext.h"

class ScenePanel;
class ProjectPanel;
class InspectorPanel;
class ViewPanel;
class MenuBarPanel;

class EditorLayer : public ZEN::Layer {
public:
    EditorLayer(ZEN::ZEngine* engine);
    void onEvent(ZEN::Event& event) override;
    bool onRunPlayMode(ZEN::RunPlayModeEvent& e);
    ~EditorLayer() override;
private:
    SelectionContext m_SelectionContext;
    InspectorPanel* m_InspectorPanel;
    ProjectPanel* m_ProjectPanel;
    MenuBarPanel* m_MenuBarPanel;
    ScenePanel* m_ScenePanel;
    ViewPanel* m_ViewPanel;
};
