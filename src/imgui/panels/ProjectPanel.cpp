
#include "ProjectPanel.h"

#include <imgui.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/MeshLibrary.h>

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
    // Example: iterate over loaded assets
    /*auto& textures = m_Renderer->getResourceManager()->getAllTextures();
    if (ImGui::TreeNode("Textures")) {
        for (auto& [id, tex] : textures) {
            if (ImGui::Selectable(tex.name.c_str())) {
                // Optionally assign selected texture to the currently selected entity
            }
        }
        ImGui::TreePop();
    }*/

    auto& meshes = ZEN::MeshLibrary::getAll();
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
