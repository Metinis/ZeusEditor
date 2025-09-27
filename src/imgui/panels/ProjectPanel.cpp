
#include "ProjectPanel.h"
#include <imgui.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/ModelLibrary.h>
#include <ZeusEngineCore/Scene.h>


ProjectPanel::ProjectPanel(ZEN::Scene *scene, ZEN::ModelLibrary *modelLibrary)
: m_Scene(scene), m_ModelLibrary(modelLibrary) {

}

void ProjectPanel::onImGuiRender(){
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, displaySize.y * 0.7f)); // bottom-left
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y * 0.3f));

    ImGui::Begin("Project Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        m_Scene->getDispatcher().trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Project"}
        );
    }

    auto& meshes = m_ModelLibrary->getAllMeshes();
    if (ImGui::TreeNode("Meshes")) {
        for (auto& [name, mesh] : meshes) {
            if (ImGui::Selectable(name.c_str())) {
                // Optionally assign selected mesh to the selected entity
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();
}
