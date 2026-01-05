#pragma once
#include <ZeusEngine.h>

#include "SelectionContext.h"

namespace ZED {
    class EditorApp;
}
struct PendingReparent {
    ZEN::UUID child;
    ZEN::UUID newParent; //null UUID = unparent
};

class ScenePanel : public ZEN::Layer  {
public:
    explicit ScenePanel(ZEN::ZEngine* engine, SelectionContext& selection);
    void drawEntityNode(ZEN::Entity& entity);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    std::optional<PendingReparent> m_PendingReparent;
    ZEN::ZEngine* m_Engine{};
    SelectionContext& m_SelectionContext;
};
