#include "InspectorPanel.h"
#include <ZeusEngineCore/ModelLibrary.h>
#include <ZeusEngineCore/Components.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/ZEngine.h>
#include <ZeusEngineCore/Scene.h>
#include <ZeusEngineCore/Entity.h>
#include <imgui.h>

#include "ZeusEngineCore/EventDispatcher.h"


static auto const inspectTransform = [](ZEN::TransformComp &out) {
    ImGui::DragFloat3("position", &out.localPosition.x, 0.01f);
    ImGui::DragFloat3("rotation", &out.localRotation.x);
    ImGui::DragFloat3("scale", &out.localScale.x, 0.01f, 0.0f, 100.0f);
};

InspectorPanel::InspectorPanel(ZEN::ZEngine* engine) : m_Engine(engine){
    m_Engine->getDispatcher().attach<ZEN::SelectEntityEvent, InspectorPanel, &InspectorPanel::onEntitySelect>(this);
}

void InspectorPanel::onImGuiRender() {
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.8f, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f));
    ImGui::Begin("Inspector", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus();
    }
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
       m_Engine->getDispatcher().trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{.panel = "Inspector"}
        );
    }
    if (m_SelectedEntity.isValid()) {
        if (auto name = m_SelectedEntity.tryGetComponent<ZEN::TagComp>()) {
            char buffer[128];
            strncpy(buffer, name->tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                name->tag = buffer;
            }
        }

        if (auto *transform = m_SelectedEntity.tryGetComponent<ZEN::TransformComp>()) {
            inspectTransform(*transform);
        }

        if (auto *material = m_SelectedEntity.tryGetComponent<ZEN::MaterialComp>()) {
            ImGui::SeparatorText("Material");

            ImGui::DragFloat("Specular", &material->specular, 0.01f, 0.0f, 1.0f);
            ImGui::DragInt("Shininess", &material->shininess, 1, 1, 256);

            // Shader selection
            if (ImGui::TreeNode("Shader")) {
                ImGui::Text("Current Shader ID: %u", material->shaderID);
                ImGui::TreePop();
            }

            // Texture selection
            if (ImGui::TreeNode("Texture")) {
                if (!material->textureIDs.empty()) {
                    int texID = static_cast<int>(material->textureIDs[0]);
                    if (ImGui::InputInt("Diffuse Tex ID", &texID)) {
                        // Update the actual texture ID
                        material->textureIDs[0] = static_cast<uint32_t>(texID);
                    }
                }

                if (!material->specularTexIDs.empty()) {
                    int texID = static_cast<int>(material->specularTexIDs[0]);
                    if (ImGui::InputInt("Specular Tex ID", &texID)) {
                        material->specularTexIDs[0] = static_cast<uint32_t>(texID);
                    }
                }
                ImGui::TreePop();
            }

        }

        // --- MeshComp ---
        if (auto *meshComp = m_SelectedEntity.tryGetComponent<ZEN::MeshComp>()) {
            ImGui::SeparatorText("Mesh");

            // Show current mesh name
            const auto &meshes = m_Engine->getModelLibrary().getAllMeshes();
            static std::string selectedMesh;
            // find current mesh name from pointer
            for (auto &[name, mesh]: meshes) {
                if (name == meshComp->name) {
                    selectedMesh = name;
                    break;
                }
            }

            if (ImGui::BeginCombo("Mesh", selectedMesh.c_str())) {
                for (auto &[name, mesh]: meshes) {
                    bool isSelected = (selectedMesh == name);
                    if (ImGui::Selectable(name.c_str(), isSelected)) {
                        meshComp->indices = mesh->indices;
                        meshComp->vertices = mesh->vertices;
                        meshComp->name = mesh->name;
                        m_SelectedEntity.removeComponent<ZEN::MeshDrawableComp>();
                        selectedMesh = name;
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

        if (m_SelectedEntity.isValid()) {
            ImGui::Separator();

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponentPopup");

            if (ImGui::BeginPopup("AddComponentPopup")) {
                if (!m_SelectedEntity.hasComponent<ZEN::MaterialComp>()) {
                    if (ImGui::MenuItem("Material")) {
                        m_SelectedEntity.addComponent<ZEN::MaterialComp>();
                        ImGui::CloseCurrentPopup();
                    }
                }

                if (!m_SelectedEntity.hasComponent<ZEN::TransformComp>()) {
                    if (ImGui::MenuItem("Transform")) {
                        m_SelectedEntity.addComponent<ZEN::TransformComp>();
                        ImGui::CloseCurrentPopup();
                    }
                }
                if (!m_SelectedEntity.hasComponent<ZEN::MeshComp>()) {
                    if (ImGui::MenuItem("Mesh")) {
                        m_SelectedEntity.addComponent<ZEN::MeshComp>();
                        ImGui::CloseCurrentPopup();
                    }
                }


                ImGui::EndPopup();
            }
        }
    }

    ImGui::End();
}

void InspectorPanel::onEntitySelect(ZEN::SelectEntityEvent &e) {
    m_SelectedEntity = e.entity;
}
