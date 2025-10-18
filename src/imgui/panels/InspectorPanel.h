
#pragma once
#include <ZeusEngineCore/Entity.h>

namespace ZEN {
    struct SelectEntityEvent;
    struct SelectMaterialEvent;
    struct Material;
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
    void onMaterialSelect(ZEN::SelectMaterialEvent& e);
private:
    void editMesh();
    void editComponents();
    void editMaterialProps();
    void editMaterialComp();
    void inspectEntity();
    void inspectMaterial();
    void renderTextureDrop(uint32_t& textures, const char* name);
    void handleMaterialDrop(const ImGuiPayload* payload);
    void handleMeshDrop(const ImGuiPayload* payload);
    void handleTextureDrop(const ImGuiPayload *payload, uint32_t& outTexture);
    ZEN::ZEngine* m_Engine{};
    ZEN::Entity m_SelectedEntity;
    ZEN::Material* m_SelectedMaterial{};
};
