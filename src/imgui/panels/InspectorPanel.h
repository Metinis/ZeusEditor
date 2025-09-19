
#pragma once
#include <entt.hpp>

class InspectorPanel {
public:
    void onImGuiRender(entt::dispatcher& dispatcher, entt::registry& registry);
    void setSelectedEntity(entt::entity entity) {m_SelectedEntity = entity;}
    entt::entity getSelectedEntity() {return m_SelectedEntity;}
private:
    entt::entity m_SelectedEntity = entt::null;
};
