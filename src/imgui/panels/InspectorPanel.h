
#pragma once
#include <entt.hpp>

namespace ZEN {
    struct SelectEntityEvent;
    class ZEngine;
}

class InspectorPanel {
public:
    explicit InspectorPanel(ZEN::ZEngine* engine);
    void onImGuiRender();
    void setSelectedEntity(entt::entity entity) {m_SelectedEntity = entity;}
    entt::entity getSelectedEntity() {return m_SelectedEntity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
private:
    ZEN::ZEngine* m_Engine{};
    entt::entity m_SelectedEntity{entt::null};
};
