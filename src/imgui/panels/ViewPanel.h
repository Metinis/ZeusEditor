
#pragma once
#include <entt.hpp>
#include <imgui.h>
#include <ZeusEngineCore/Layer.h>

namespace ZEN {
    class ZEngine;
}
class ViewPanel : public ZEN::Layer  {
public:
    explicit ViewPanel(ZEN::ZEngine* engine);
    void onUIRender() override;

private:
    ZEN::ZEngine* m_Engine{};
    ImVec2 m_PanelSize;
};
