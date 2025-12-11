#pragma once
#include <ZeusEngine.h>
#include "SelectionContext.h"

class MenuBarPanel : public ZEN::Layer  {
public:
    explicit MenuBarPanel(ZEN::ZEngine* engine, SelectionContext& selection);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    //void onToggleEditor(ZEN::ToggleEditorEvent& e);
    bool m_isPLaying{ false };
    ZEN::ZEngine* m_Engine{};
    SelectionContext& m_SelectionContext;
};
