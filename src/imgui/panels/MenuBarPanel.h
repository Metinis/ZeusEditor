
#pragma once

namespace ZEN {
    class ZEngine;
}

class MenuBarPanel {
public:
    explicit MenuBarPanel(ZEN::ZEngine* engine);
    void onImGuiRender();
private:
    ZEN::ZEngine* m_Engine{};
};
