#pragma once
#include <ZeusEngine.h>
#include "SelectionContext.h"

class ProjectPanel : public ZEN::Layer  {
public:
    explicit ProjectPanel(ZEN::ZEngine* engine, SelectionContext& selection);
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
    SelectionContext& m_SelectionContext;
    ZEN::ZEngine* m_Engine{};
};
