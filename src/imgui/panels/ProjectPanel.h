
#pragma once
#include <entt.hpp>

namespace ZEN {
    class ZEngine;
}
class ProjectPanel {
public:
    explicit ProjectPanel(ZEN::ZEngine* engine);
    void onImGuiRender();
private:
    ZEN::ZEngine* m_Engine{};
};
