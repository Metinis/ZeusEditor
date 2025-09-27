
#pragma once
#include <entt.hpp>

namespace ZEN {
    struct SelectEntityEvent;
    class ModelLibrary;
    class Scene;
}

class ScenePanel {
public:
    explicit ScenePanel(ZEN::Scene* scene, ZEN::ModelLibrary* modelLibrary);
    void drawEntityNode(entt::entity entity);
    void onImGuiRender();
    void setSelectedEntity(entt::entity entity) {m_SelectedEntity = entity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
    entt::entity getSelectedEntity() {return m_SelectedEntity;}
private:
    ZEN::Scene* m_Scene;
    ZEN::ModelLibrary* m_ModelLibrary{};
    entt::entity m_SelectedEntity = entt::null;
};
