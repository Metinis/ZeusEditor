
#include "ProjectPanel.h"

#include <imgui.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/ModelLibrary.h>

void ProjectPanel::onImGuiRender(entt::dispatcher &dispatcher) {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, displaySize.y * 0.7f)); // bottom-left
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y * 0.3f));

    ImGui::Begin("Project Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        dispatcher.trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Project"}
        );
    }

    auto& meshes = ZEN::ModelLibrary::getAll();
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
