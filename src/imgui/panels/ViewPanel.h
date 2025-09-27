
#pragma once
#include <entt.hpp>
#include <imgui.h>

namespace ZEN {
    class Scene;
}
class ViewPanel {
public:
    explicit ViewPanel(ZEN::Scene* scene, void* colorTex);
    void onImGuiRender();

private:
    ZEN::Scene* m_Scene;
    ImVec2 m_PanelSize;
    void* m_ColorTexture;
};
