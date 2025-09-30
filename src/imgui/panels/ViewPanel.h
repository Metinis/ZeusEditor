
#pragma once
#include <entt.hpp>
#include <imgui.h>

namespace ZEN {
    class ZEngine;
}
class ViewPanel {
public:
    explicit ViewPanel(ZEN::ZEngine* engine);
    void onImGuiRender();

private:
    ZEN::ZEngine* m_Engine{};
    ImVec2 m_PanelSize;
};
