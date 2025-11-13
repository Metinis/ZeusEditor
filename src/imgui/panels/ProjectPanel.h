#pragma once
#include <ZeusEngine.h>

class ProjectPanel : public ZEN::Layer  {
public:
    explicit ProjectPanel(ZEN::ZEngine* engine);
    void onUIRender() override;
private:
    void drawFolderTree();
    void drawAssetGrid();
    void drawContextMenu();
    void drawMeshesGrid();
    void drawMaterialsGrid();
    void drawTexturesGrid();
    std::string m_SelectedFolder{};
    ZEN::ZEngine* m_Engine{};
};
