#pragma once
#include <memory>
#include <ZeusEngineCore/Layer.h>
#include "../imgui/panels/MenuBarPanel.h"
#include "../imgui/panels/InspectorPanel.h"
#include "../imgui/panels/ScenePanel.h"
#include "../imgui/panels/ProjectPanel.h"
#include "../imgui/panels/ViewPanel.h"

namespace ZEN {
    class ZEngine;
}

class MenuBarPanel;
class ViewPanel;
class InspectorPanel;
class ProjectPanel;
class ScenePanel;

class Editor : ZEN::Layer {
public:
    Editor(ZEN::ZEngine* engine);
    void onUpdate() override;
    void onUIRender() override;
private:
    ZEN::ZEngine* m_Engine{};
    std::unique_ptr<MenuBarPanel> m_MenuBarPanel{};
    std::unique_ptr<ViewPanel> m_ViewPanel{};
    std::unique_ptr<InspectorPanel> m_InspectorPanel{};
    std::unique_ptr<ProjectPanel> m_ProjectPanel{};
    std::unique_ptr<ScenePanel> m_ScenePanel{};
};
