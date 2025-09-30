
#pragma once
#include <entt.hpp>

namespace ZEN {
    struct SelectEntityEvent;
    class ZEngine;
    class Scene;
}

class ScenePanel {
public:
    explicit ScenePanel(ZEN::ZEngine* engine);
    void drawEntityNode(entt::entity entity);
    void onImGuiRender();
    void setSelectedEntity(entt::entity entity) {m_SelectedEntity = entity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
    entt::entity getSelectedEntity() {return m_SelectedEntity;}
private:
    ZEN::ZEngine* m_Engine{};
    entt::entity m_SelectedEntity = entt::null;
};
