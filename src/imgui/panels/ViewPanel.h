#pragma once
#include <ZeusEngine.h>

class ViewPanel : public ZEN::Layer  {
public:
    explicit ViewPanel(ZEN::ZEngine* engine);
    void onUIRender() override;

private:
    ZEN::ZEngine* m_Engine{};
    ImVec2 m_PanelSize;
};
