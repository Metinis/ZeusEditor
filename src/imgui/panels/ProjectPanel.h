
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
    std::string m_SelectedFolder{};
    ZEN::ZEngine* m_Engine{};
};
