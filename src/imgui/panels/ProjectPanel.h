#pragma once
#include <ZeusEngine.h>
#include "SelectionContext.h"

namespace ZEN {
    class VKRenderer;
}
class ProjectPanel : public ZEN::Layer  {
public:
    explicit ProjectPanel(ZEN::EngineContext* ctx, SelectionContext &selection);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    //void onToggleEditor(ZEN::ToggleEditorEvent& e);
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    void drawFolderTree();
    void drawAssetGrid();
    void drawContextMenu();
    void drawMeshesGrid();
    void drawMaterialsGrid();
    void drawTexturesGrid();

    void createMaterialPopup();
    SelectionContext& m_SelectionContext;
    bool m_OpenCreateMaterialPopup = false;
    std::shared_ptr<ZEN::AssetLibrary> m_AssetLibrary{};
    ZEN::ModelImporter* m_Importer{};
    ZEN::VKRenderer* m_Renderer{};
};
