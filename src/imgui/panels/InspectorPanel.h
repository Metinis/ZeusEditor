#pragma once
#include <ZeusEngine.h>
#include "SelectionContext.h"

struct ImGuiPayload;

class InspectorPanel : public ZEN::Layer {
public:
    explicit InspectorPanel(ZEN::ZEngine* engine, SelectionContext& selection);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    void editMesh();
    void editComponents();
    void editMaterialProps();
    void editMaterialComp();
    void inspectEntity();
    void inspectMaterial();
    void renderTextureDrop(ZEN::AssetID& textureID, const char* name);
    void handleMaterialDrop(const ImGuiPayload* payload);
    void handleMeshDrop(const ImGuiPayload* payload);
    void handleTextureDrop(const ImGuiPayload *payload, ZEN::AssetID& outTexture);
    ZEN::ZEngine* m_Engine{};
    std::shared_ptr<ZEN::AssetLibrary> m_AssetLibrary;
    SelectionContext& m_SelectionContext;
};
