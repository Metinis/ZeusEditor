#pragma once
#include <ZeusEngine.h>

#include "SelectionContext.h"

class ViewPanel : public ZEN::Layer  {
public:
    explicit ViewPanel(ZEN::ZEngine* engine, SelectionContext& selection);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    bool m_EditorToggled{true};
    ZEN::ZEngine* m_Engine{};
    SelectionContext& m_SelectionContext;
    ImVec2 m_PanelSize;
};
