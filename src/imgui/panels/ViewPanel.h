#pragma once
#include <ZeusEngine.h>

#include "ImGuizmo.h"
#include "SelectionContext.h"

class ViewPanel : public ZEN::Layer  {
public:
    explicit ViewPanel(ZEN::ZEngine* engine, SelectionContext& selection);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    void drawGizmo();
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    bool onKeyPressedEvent(ZEN::KeyPressedEvent &e);
    bool onMouseButtonPressedEvent(ZEN::MouseButtonPressedEvent &e);
    bool onMouseButtonReleasedEvent(ZEN::MouseButtonReleasedEvent &e);
    bool onMouseMovedEvent(ZEN::MouseMovedEvent &e);
    ImGuizmo::OPERATION m_GizmoType{ImGuizmo::SCALE};
    ImGuizmo::MODE m_GizmoMode{ImGuizmo::WORLD};
    bool m_EditorToggled{true};
    bool m_ImGuiWantsMouse{false};
    bool m_ImGuiWantsKeyboard{false};
    bool m_IsViewportHovered{false};
    bool m_IsFocused{false};
    ZEN::ZEngine* m_Engine{};
    SelectionContext& m_SelectionContext;
    ImVec2 m_PanelSize;
};
