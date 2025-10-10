
#pragma once
#include <entt.hpp>

namespace ZEN {
    class ZEngine;
}
class ProjectPanel {
public:
    explicit ProjectPanel(ZEN::ZEngine* engine);
    void onImGuiRender();
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
