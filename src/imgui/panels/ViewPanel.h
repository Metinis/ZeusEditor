#pragma once
#include <ZeusEngine.h>

#include "ImGuizmo.h"
#include "SelectionContext.h"

class ViewPanel : public ZEN::Layer  {
public:
    explicit ViewPanel(ZEN::EngineContext* ctx, SelectionContext &selection);
    void onUIRender() override;
    void onEvent(ZEN::Event& event) override;
private:
    void drawColorImage();
    void drawGizmo();
    void handleDrop();
    bool onPlayModeEvent(ZEN::RunPlayModeEvent &e);
    bool onKeyPressedEvent(ZEN::KeyPressedEvent &e);
    bool onMouseButtonPressedEvent(ZEN::MouseButtonPressedEvent &e);
    bool onMouseButtonReleasedEvent(ZEN::MouseButtonReleasedEvent &e);
    bool onMouseMovedEvent(ZEN::MouseMovedEvent &e);
    void doMousePick();
    ImGuizmo::OPERATION m_GizmoType{ImGuizmo::TRANSLATE};
    ImGuizmo::MODE m_GizmoMode{ImGuizmo::WORLD};
    bool m_EditorToggled{true};
    bool m_ImGuiWantsMouse{false};
    bool m_ImGuiWantsKeyboard{false};
    bool m_IsViewportHovered{false};
    bool m_IsFocused{false};
    bool m_DoMousePick{false};
    glm::vec2 m_ViewportBounds[2];
    SelectionContext& m_SelectionContext;
    ZEN::VKRenderer* m_Renderer{};
    ZEN::Scene* m_Scene{};
    ImVec2 m_PanelSize;
};
