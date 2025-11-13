#pragma once
#include <ZeusEngine.h>

class MenuBarPanel : public ZEN::Layer  {
public:
    explicit MenuBarPanel(ZEN::ZEngine* engine);
    void onUIRender() override;
private:
    ZEN::ZEngine* m_Engine{};
};
