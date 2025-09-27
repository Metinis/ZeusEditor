
#pragma once
#include <entt.hpp>

namespace ZEN {
    class Scene;
    class ModelLibrary;
}
class ProjectPanel {
public:
    explicit ProjectPanel(ZEN::Scene* scene, ZEN::ModelLibrary* modelLibrary);
    void onImGuiRender();
private:
    ZEN::ModelLibrary* m_ModelLibrary{};
    ZEN::Scene* m_Scene{};
};
