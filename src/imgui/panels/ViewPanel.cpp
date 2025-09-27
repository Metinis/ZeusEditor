
#include "ViewPanel.h"
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/Scene.h>

ViewPanel::ViewPanel(ZEN::Scene* scene, void* colorTex)
: m_Scene(scene), m_ColorTexture(colorTex){
    m_PanelSize = {800, 600};
}

void ViewPanel::onImGuiRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.2f, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.6f, displaySize.y * 0.7f));

    ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse);

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        m_Scene->getDispatcher().trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Scene View" }
        );
    }

    ImVec2 newSize = ImGui::GetContentRegionAvail();
    if (m_PanelSize.x != newSize.x || m_PanelSize.y != newSize.y) {
        m_PanelSize = newSize;
        m_Scene->getDispatcher().trigger<ZEN::SceneViewResizeEvent>(
            ZEN::SceneViewResizeEvent{ newSize.x, newSize.y }
        );
    }
    ImGui::Image(
        (void*)(intptr_t)m_ColorTexture,
        m_PanelSize,
        ImVec2(0, 1), // uv0 (top-left)
        ImVec2(1, 0)  // uv1 (bottom-right)
    );

    ImGui::End();
}