
#pragma once
#include <entt.hpp>

namespace ZEN {
    struct SelectEntityEvent;
    class Scene;
    class ModelLibrary;
}

class InspectorPanel {
public:
    explicit InspectorPanel(ZEN::Scene* scene, ZEN::ModelLibrary* modelLibrary);
    void onImGuiRender();
    void setSelectedEntity(entt::entity entity) {m_SelectedEntity = entity;}
    entt::entity getSelectedEntity() {return m_SelectedEntity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
private:
    ZEN::Scene* m_Scene{};
    ZEN::ModelLibrary* m_ModelLibrary{};
    entt::entity m_SelectedEntity{entt::null};
};
