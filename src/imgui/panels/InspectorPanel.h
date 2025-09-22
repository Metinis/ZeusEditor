
#pragma once
#include <entt.hpp>

namespace ZEN {
    struct SelectEntityEvent;
}

class InspectorPanel {
public:
    explicit InspectorPanel(entt::dispatcher& dispatcher);
    void onImGuiRender(entt::dispatcher& dispatcher, entt::registry& registry);
    void setSelectedEntity(entt::entity entity) {m_SelectedEntity = entity;}
    entt::entity getSelectedEntity() {return m_SelectedEntity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
private:
    entt::entity m_SelectedEntity = entt::null;
};
