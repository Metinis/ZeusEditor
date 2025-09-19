
#include "InspectorPanel.h"
#include <ZeusEngineCore/MeshLibrary.h>

#include <imgui.h>
#include <ZeusEngineCore/Components.h>
#include <ZeusEngineCore/InputEvents.h>

static auto const inspectTransform = [](ZEN::TransformComp& out) {
    ImGui::DragFloat3("position", &out.position.x, 0.01f);
    ImGui::DragFloat3("rotation", &out.rotation.x);
    ImGui::DragFloat3("scale", &out.scale.x, 0.01f, 0.0f, 100.0f);
};
void InspectorPanel::onImGuiRender(entt::dispatcher& dispatcher, entt::registry& registry) {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.8f, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f));
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        dispatcher.trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Inspector" }
        );
    }
    if (m_SelectedEntity != entt::null && registry.valid(m_SelectedEntity)) {
        if (auto* name = registry.try_get<ZEN::TagComp>(m_SelectedEntity)) {
            char buffer[128];
            strncpy(buffer, name->tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                name->tag = buffer;
            }
        }

        if (auto* transform = registry.try_get<ZEN::TransformComp>(m_SelectedEntity)) {
            inspectTransform(*transform);
        }

        if (auto* material = registry.try_get<ZEN::MaterialComp>(m_SelectedEntity)) {
        ImGui::SeparatorText("Material");

        ImGui::DragFloat("Specular", &material->specular, 0.01f, 0.0f, 1.0f);
        ImGui::DragInt("Shininess", &material->shininess, 1, 1, 256);

        // Shader selection
        if (ImGui::TreeNode("Shader")) {
            // you could fetch list from resource manager if you expose it:
            // auto& shaders = m_Renderer->getResourceManager()->getAllShaders();
            // for (auto& [id, shader] : shaders) ...
            ImGui::Text("Current Shader ID: %u", material->shaderID);
            ImGui::TreePop();
        }

        // Texture selection
        if (ImGui::TreeNode("Textures")) {
            ImGui::Text("Diffuse Tex ID: %u", material->textureID);
            ImGui::Text("Specular Tex ID: %u", material->specularTexID);
            // Here you can add an ImGui::Selectable list from your resource manager
            ImGui::TreePop();
        }
    }

    // --- MeshComp ---
    if (auto* meshComp = registry.try_get<ZEN::MeshComp>(m_SelectedEntity)) {
        ImGui::SeparatorText("Mesh");

        // Show current mesh name
        const auto& meshes = ZEN::MeshLibrary::getAll();
        static std::string selectedMesh;
        // find current mesh name from pointer
        for (auto& [name, mesh] : meshes) {
           // if (&mesh == &(*meshComp)) {
            //    selectedMesh = name;
                break;
            //}
        }

        if (ImGui::BeginCombo("Mesh", selectedMesh.c_str())) {
            for (auto& [name, mesh] : meshes) {
                bool isSelected = (selectedMesh == name);
                if (ImGui::Selectable(name.c_str(), isSelected)) {
                    //*meshComp = mesh; // replace the mesh data
                    selectedMesh = name;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

        if (m_SelectedEntity != entt::null && registry.valid(m_SelectedEntity)) {
            ImGui::Separator();

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponentPopup");

            if (ImGui::BeginPopup("AddComponentPopup")) {

                if (!registry.all_of<ZEN::MaterialComp>(m_SelectedEntity)) {
                    if (ImGui::MenuItem("Material")) {
                        registry.emplace<ZEN::MaterialComp>(m_SelectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                }

                if (!registry.all_of<ZEN::TransformComp>(m_SelectedEntity)) {
                    if (ImGui::MenuItem("Transform")) {
                        registry.emplace<ZEN::TransformComp>(m_SelectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }
        }

    }

    ImGui::End();
}
