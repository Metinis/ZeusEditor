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
    explicit ScenePanel(ZEN::EngineContext* ctx, SelectionContext &selection);
    void drawEntityNode(ZEN::Entity& entity);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    std::optional<PendingReparent> m_PendingReparent;
    SelectionContext& m_SelectionContext;
    ZEN::Scene* m_Scene{};
};
