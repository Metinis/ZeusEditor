
#pragma once
#include <ZeusEngineCore/Entity.h>
#include <ZeusEngineCore/Layer.h>

namespace ZEN {
    struct SelectEntityEvent;
    class ZEngine;
    class Scene;
}

class ScenePanel : public ZEN::Layer  {
public:
    explicit ScenePanel(ZEN::ZEngine* engine);
    void drawEntityNode(ZEN::Entity& entity);
    void onUIRender() override;
    void setSelectedEntity(ZEN::Entity entity) {m_SelectedEntity = entity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
    ZEN::Entity getSelectedEntity() {return m_SelectedEntity;}
private:
    ZEN::ZEngine* m_Engine{};
    ZEN::Entity m_SelectedEntity;
};
