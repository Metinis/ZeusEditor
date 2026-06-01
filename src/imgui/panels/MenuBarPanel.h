#pragma once
#include <ZeusEngine.h>
#include "SelectionContext.h"

class MenuBarPanel : public ZEN::Layer  {
public:
    explicit MenuBarPanel(ZEN::EngineContext* ctx, SelectionContext &selection);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    //void onToggleEditor(ZEN::ToggleEditorEvent& e);
    bool m_isPLaying{ false };
    SelectionContext& m_SelectionContext;
    ZEN::Scene* m_Scene;
    ZEN::CameraSystem* m_CameraSystem;
    ZEN::CompRegistry* m_CompRegistry;
    ZEN::SystemManager* m_SystemManager;
};
