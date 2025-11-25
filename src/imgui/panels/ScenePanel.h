#pragma once
#include <ZeusEngine.h>

#include "SelectionContext.h"

namespace ZED {
    class EditorApp;
}

class ScenePanel : public ZEN::Layer  {
public:
    explicit ScenePanel(ZEN::ZEngine* engine, SelectionContext& selection);
    void drawEntityNode(ZEN::Entity& entity);
    void onUIRender() override;
private:
    ZEN::ZEngine* m_Engine{};
    SelectionContext& m_SelectionContext;
};
