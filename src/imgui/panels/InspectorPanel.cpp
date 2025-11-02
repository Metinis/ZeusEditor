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

InspectorPanel::InspectorPanel(ZEN::ZEngine *engine) : m_Engine(engine) {
    m_Engine->getDispatcher().attach<ZEN::SelectEntityEvent, InspectorPanel, &InspectorPanel::onEntitySelect>(this);
    m_Engine->getDispatcher().attach<ZEN::SelectMaterialEvent, InspectorPanel, &InspectorPanel::onMaterialSelect>(this);
}
void InspectorPanel::editMesh() {
    if (auto *meshComp = m_SelectedEntity.tryGetComponent<ZEN::MeshComp>()) {
        ImGui::SeparatorText("Mesh");


        const auto &meshes = m_Engine->getModelLibrary().getAllMeshes();
        static std::string selectedMesh;

        for (auto &[name, mesh]: meshes) {
            if (name == meshComp->name) {
                selectedMesh = name;
                break;
            }
        }
        ImGui::BeginGroup();
        //ImGui::PushID("Mesh")

        float buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().FramePadding.x * 2;
        float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
        float comboWidth = ImGui::GetContentRegionAvail().x - buttonWidth - itemSpacing;

        ImGui::SetNextItemWidth(comboWidth);
        if (ImGui::BeginCombo("##mesh", selectedMesh.c_str())) {
            for (auto &[name, mesh] : meshes) {
                bool isSelected = (selectedMesh == name);
                if (ImGui::Selectable(name.c_str(), isSelected)) {
                    meshComp->name = name;
                    //remove current drawable for regenaration
                    if(m_SelectedEntity.hasComponent<ZEN::MeshDrawableComp>())
                        m_SelectedEntity.removeComponent<ZEN::MeshDrawableComp>();
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


        ImGui::SameLine();
        ImGui::PushID("RemoveMesh");
        if (ImGui::Button("X")) {
            m_SelectedEntity.removeComponent<ZEN::MeshComp>();
        }
        ImGui::PopID();

        ImGui::EndGroup();


    }
}
void InspectorPanel::editComponents() {
    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponentPopup");

    if (ImGui::BeginPopup("AddComponentPopup")) {
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
    ImGui::BeginChild("Drop", ImVec2(0, 200), true);
    ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("MATERIAL_NAME")) {
            handleMaterialDrop(payload);
        }
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("MESH_NAME")) {
            handleMeshDrop(payload);
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::EndChild();
}
void InspectorPanel::handleTextureDrop(const ImGuiPayload *payload, uint32_t& outTexture) {
    const char *data = (const char *) payload->Data;
    auto texture = m_Engine->getModelLibrary().getTexture(data);
    outTexture = texture;
}
void InspectorPanel::renderTextureDrop(uint32_t& texture, const char* name) {
    constexpr float thumbnailSize = 8.0f;

    int texID = static_cast<int>(texture);
    ImGui::ImageButton(name, (void*)m_Engine->getRenderer().getResourceManager()->getTexture(texID),
        ImVec2(thumbnailSize, thumbnailSize), ImVec2(0,1), ImVec2(1,0));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("TEXTURE_NAME")) {
            handleTextureDrop(payload, texture);
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::SameLine();
    ImGui::Text(name);
}
void InspectorPanel::editMaterialComp() {
    if (auto *materialComp = m_SelectedEntity.tryGetComponent<ZEN::MaterialComp>()) {
        ImGui::SeparatorText("Material");


        const auto &materials = m_Engine->getModelLibrary().getAllMaterials();
        static std::string selectedMaterial;

        for (auto &[name, material] : materials) {
            if (name == materialComp->name) {
                selectedMaterial = name;
                break;
            }
        }
        ImGui::BeginGroup();

        float buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().FramePadding.x * 2;
        float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
        float comboWidth = ImGui::GetContentRegionAvail().x - buttonWidth - itemSpacing;

        ImGui::SetNextItemWidth(comboWidth);
        if (ImGui::BeginCombo("##material", selectedMaterial.c_str())) {
            for (auto &[name, mesh] : materials) {
                bool isSelected = (selectedMaterial == name);
                if (ImGui::Selectable(name.c_str(), isSelected)) {
                    materialComp->name = name;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


        ImGui::SameLine();
        ImGui::PushID("RemoveMat");
        if (ImGui::Button("X")) {
            //reset to default if has mesh, otherwise allow removal
            if(m_SelectedEntity.hasComponent<ZEN::MeshDrawableComp>()) {
                m_SelectedEntity.getComponent<ZEN::MaterialComp>() = ZEN::MaterialComp{.name = "Default"};
            }
            else {
                m_SelectedEntity.removeComponent<ZEN::MaterialComp>();
            }
        }
        ImGui::PopID();

        ImGui::EndGroup();


    }
}


void InspectorPanel::editMaterialProps() {
    ImGui::SeparatorText("Material");

    ImGui::DragFloat3("Albedo", &m_SelectedMaterial->albedo.x, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Metallic", &m_SelectedMaterial->metallic, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Roughness", &m_SelectedMaterial->roughness, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Ambient Oclussion", &m_SelectedMaterial->ao, 0.01f, 0.0f, 1.0f);
    ImGui::Checkbox("Is Metal", &m_SelectedMaterial->metal);

    if (ImGui::TreeNode("Shader")) {
        ImGui::Text("Current Shader ID: %u", m_SelectedMaterial->shaderID);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Texture")) {
        renderTextureDrop(m_SelectedMaterial->textureID, "Albedo");
        renderTextureDrop(m_SelectedMaterial->metallicTexID, "Metallic");
        renderTextureDrop(m_SelectedMaterial->roughnessTexID, "Roughness");
        renderTextureDrop(m_SelectedMaterial->AOTexID, "AO");
        renderTextureDrop(m_SelectedMaterial->normalTexID, "Normal");
        ImGui::TreePop();
    }
}

void InspectorPanel::inspectEntity() {
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

    editMesh();

    editMaterialComp();

    ImGui::Separator();

    editComponents();


}
void InspectorPanel::inspectMaterial() {
    editMaterialProps();
}

void InspectorPanel::onImGuiRender() {
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    float menuBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
    ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.8f, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f - menuBarHeight));
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
    if(m_SelectedEntity.isValid()) {
        inspectEntity();
    }
    else if(m_SelectedMaterial) {
        inspectMaterial();
    }

    ImGui::End();
}

void InspectorPanel::onEntitySelect(ZEN::SelectEntityEvent &e) {
    m_SelectedEntity = e.entity;
    m_SelectedMaterial = nullptr;
}

void InspectorPanel::onMaterialSelect(ZEN::SelectMaterialEvent &e) {
    m_SelectedMaterial = m_Engine->getModelLibrary().getMaterial(e.materialName);
    m_SelectedEntity = ZEN::Entity();
}

void InspectorPanel::handleMaterialDrop(const ImGuiPayload *payload) {
    const char *data = (const char *) payload->Data;
    //auto material = m_Engine->getModelLibrary().getMaterial(data);

    std::cout << "Dropped Material \n";

    if (!m_SelectedEntity.hasComponent<ZEN::MaterialComp>())
        m_SelectedEntity.addComponent<ZEN::MaterialComp>(ZEN::MaterialComp{.name = data});
    else
        m_SelectedEntity.getComponent<ZEN::MaterialComp>() = ZEN::MaterialComp{.name = data};
}

void InspectorPanel::handleMeshDrop(const ImGuiPayload *payload) {
    const char *data = (const char *) payload->Data;
    //auto mesh = m_Engine->getModelLibrary().getMesh(data);

    std::cout << "Dropped Mesh \n";

    if (!m_SelectedEntity.hasComponent<ZEN::MeshComp>()) {
        m_SelectedEntity.addComponent<ZEN::MeshComp>(ZEN::MeshComp{.name = data});
    } else {
        m_SelectedEntity.getComponent<ZEN::MeshComp>() = ZEN::MeshComp{.name = data};
        if (m_SelectedEntity.hasComponent<ZEN::MeshDrawableComp>())
            m_SelectedEntity.removeComponent<ZEN::MeshDrawableComp>();
    }
}
