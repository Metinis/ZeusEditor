
#pragma once
#include <ZeusEngineCore/Entity.h>

namespace ZEN {
    struct SelectEntityEvent;
    class ZEngine;
    class Entity;
}
struct ImGuiPayload;

class InspectorPanel {
public:
    explicit InspectorPanel(ZEN::ZEngine* engine);
    void onImGuiRender();
    void setSelectedEntity(ZEN::Entity entity) {m_SelectedEntity = entity;}
    ZEN::Entity getSelectedEntity() {return m_SelectedEntity;}
    void onEntitySelect(ZEN::SelectEntityEvent& e);
private:
    void handleMaterialDrop(const ImGuiPayload* payload);
    void handleMeshDrop(const ImGuiPayload* payload);
    ZEN::ZEngine* m_Engine{};
    ZEN::Entity m_SelectedEntity;
};
