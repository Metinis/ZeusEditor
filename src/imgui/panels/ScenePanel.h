
#pragma once
#include <entt.hpp>

namespace ZEN {
    struct SelectEntityEvent;
}

class ScenePanel {
public:
    explicit ScenePanel(entt::dispatcher& dispatcher);
    void onImGuiRender(entt::dispatcher& dispatcher, entt::registry& registry);
    void setSelectedEntity(entt::entity entity) {m_SelectedEntity = entity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
    entt::entity getSelectedEntity() {return m_SelectedEntity;}
private:
    entt::entity m_SelectedEntity = entt::null;
};
