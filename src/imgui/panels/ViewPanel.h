
#pragma once
#include <entt.hpp>
#include <imgui.h>

class ViewPanel {
public:
    explicit ViewPanel();
    void onImGuiRender(entt::dispatcher& dispatcher, void* colorTexture);

private:
    ImVec2 m_PanelSize;
};
