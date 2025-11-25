#pragma once
#include <ZeusEngine.h>
#include "SelectionContext.h"

struct ImGuiPayload;

class InspectorPanel : public ZEN::Layer {
public:
    explicit InspectorPanel(ZEN::ZEngine* engine, SelectionContext& selection);
    void onUIRender() override;
    //void onEntitySelect(ZEN::SelectEntityEvent& e);
    //void onMaterialSelect(ZEN::SelectMaterialEvent& e);
private:
    //void onToggleEditor(ZEN::ToggleEditorEvent& e);
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
    SelectionContext& m_SelectionContext;
};
