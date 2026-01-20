#include "InspectorPanel.h"

#include "glm/gtc/type_ptr.hpp"

static auto const inspectTransform = [](ZEN::TransformComp &out) {
    ImGui::DragFloat3("position", &out.localPosition.x, 0.01f);
    glm::vec3 euler = glm::degrees(glm::eulerAngles(out.localRotation));
    if (ImGui::DragFloat3("rotation", &euler.x)) {
        out.localRotation = glm::quat(glm::radians(euler));
    }
    ImGui::DragFloat3("scale", &out.localScale.x, 0.01f, 0.0f, 100.0f);
};

InspectorPanel::InspectorPanel(ZEN::ZEngine *engine, SelectionContext &selection) : m_Engine(engine),
    m_SelectionContext(selection) {
    m_AssetLibrary = ZEN::Project::getActive()->getAssetLibrary();
}

void InspectorPanel::editMesh() {
    if (auto *meshComp = m_SelectionContext.getEntity().tryGetComponent<ZEN::MeshComp>()) {
        ImGui::SeparatorText("Mesh");

        const auto &assets = m_AssetLibrary->getAll();
        static ZEN::AssetID selectedMeshID = meshComp->handle.id();

        if (selectedMeshID != meshComp->handle.id()) {
            selectedMeshID = meshComp->handle.id();
        }

        ImGui::BeginGroup();
        float buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().FramePadding.x * 2;
        float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
        float comboWidth = ImGui::GetContentRegionAvail().x - buttonWidth - itemSpacing;

        ImGui::SetNextItemWidth(comboWidth);
        if (ImGui::BeginCombo("##mesh", m_AssetLibrary->getName(selectedMeshID).c_str())) {
            for (auto &[id, asset]: assets) {
                if (!std::holds_alternative<ZEN::MeshData>(asset)) continue;

                bool isSelected = (selectedMeshID == id);
                std::string label = m_AssetLibrary->getName(id);
                if (label.empty()) {
                    label = std::to_string(id);
                }
                if (ImGui::Selectable(label.c_str(), isSelected)) {
                    meshComp->handle.setID(id);
                    selectedMeshID = id;

                    if (m_SelectionContext.getEntity().hasComponent<ZEN::MeshDrawableComp>())
                        m_SelectionContext.getEntity().removeComponent<ZEN::MeshDrawableComp>();
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::PushID("RemoveMesh");
        if (ImGui::Button("X")) {
            m_SelectionContext.getEntity().removeComponent<ZEN::MeshComp>();
        }
        ImGui::PopID();

        ImGui::EndGroup();
    }
}

void InspectorPanel::editComponents() {
    editRuntimeComps();
    editMesh();
    editMaterialComp();
    editBoxColliderComp();
    editSphereColliderComp();
    editRigidBodyComp();
    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponentPopup");

    if (ImGui::BeginPopup("AddComponentPopup")) {
        if (!m_SelectionContext.getEntity().hasComponent<ZEN::TransformComp>()) {
            if (ImGui::MenuItem("Transform")) {
                m_SelectionContext.getEntity().addComponent<ZEN::TransformComp>();
                ImGui::CloseCurrentPopup();
            }
        }
        if (!m_SelectionContext.getEntity().hasComponent<ZEN::MeshComp>()) {
            if (ImGui::MenuItem("Mesh")) {
                m_SelectionContext.getEntity().addComponent<ZEN::MeshComp>();
                ImGui::CloseCurrentPopup();
            }
        }
        if (!m_SelectionContext.getEntity().hasComponent<ZEN::BoxColliderComp>()) {
            if (ImGui::MenuItem("BoxCollider")) {
                m_SelectionContext.getEntity().addComponent<ZEN::BoxColliderComp>();
                ImGui::CloseCurrentPopup();
            }
        }
        if (!m_SelectionContext.getEntity().hasComponent<ZEN::SphereColliderComp>()) {
            if (ImGui::MenuItem("SphereCollider")) {
                m_SelectionContext.getEntity().addComponent<ZEN::SphereColliderComp>();
                ImGui::CloseCurrentPopup();
            }
        }
        if (!m_SelectionContext.getEntity().hasComponent<ZEN::RigidBodyComp>()) {
            if (ImGui::MenuItem("RigidBody")) {
                m_SelectionContext.getEntity().addComponent<ZEN::RigidBodyComp>();
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Custom Component"))
        ImGui::OpenPopup("AddCustomComponentPopup");


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

void InspectorPanel::editRuntimeComps() {
    if (ImGui::BeginPopup("AddCustomComponentPopup")) {
        for (const auto &comp: m_Engine->getCompRegistry().getComponents()) {
            if (ImGui::MenuItem(comp.name)) {
                m_SelectionContext.getEntity().addRuntimeComponent(comp);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
    for (auto &compInfo: m_Engine->getCompRegistry().getComponents()) {
        if (auto *comp = m_SelectionContext.getEntity().getRuntimeComponent(compInfo.name)) {
            if (ImGui::CollapsingHeader(compInfo.name)) {
                ImGui::SameLine();
                ImGui::PushID(std::format("RemoveCustom{}", compInfo.name).c_str());
                if (ImGui::Button("X")) {
                    m_SelectionContext.getEntity().removeRuntimeComponent(compInfo.name);
                }
                ImGui::PopID();

                for (auto &field: compInfo.fields) {
                    void *ptr = comp->getFieldPtr(field.name);

                    ImGui::Text("%s", field.name);
                    ImGui::SameLine(150);

                    switch (field.type) {
                        case ZEN::FieldType::Float: {
                            float *val = reinterpret_cast<float *>(ptr);
                            ImGui::PushID(field.name);
                            ImGui::InputFloat("", val);
                            ImGui::PopID();
                            break;
                        }
                        case ZEN::FieldType::Int: {
                            int *val = reinterpret_cast<int *>(ptr);
                            ImGui::PushID(field.name);
                            ImGui::InputInt("", val);
                            ImGui::PopID();
                            break;
                        }
                        case ZEN::FieldType::Bool: {
                            bool *val = reinterpret_cast<bool *>(ptr);
                            ImGui::PushID(field.name);
                            ImGui::Checkbox("", val);
                            ImGui::PopID();
                            break;
                        }
                    }
                }
            }
        }
    }
}

void InspectorPanel::handleTextureDrop(const ImGuiPayload *payload, ZEN::AssetID &outTexture) {
    ZEN::AssetID assetID;
    if (payload->DataSize == sizeof(ZEN::AssetID)) {
        std::memcpy(&assetID, payload->Data, sizeof(ZEN::AssetID));
    }
    outTexture = assetID;
}

void InspectorPanel::renderTextureDrop(ZEN::AssetID &textureID, const char *name) {
    constexpr float thumbnailSize = 8.0f;

    auto resourceManager = ZEN::Application::get().getEngine()->getRenderer().getResourceManager();
    int texID = resourceManager->get<ZEN::GPUTexture>(textureID)->drawableID;
    ImGui::ImageButton(
        name, reinterpret_cast<void *>(static_cast<uintptr_t>(m_Engine->getRenderer().getResourceManager()->
            getTexture(texID))),
        ImVec2(thumbnailSize, thumbnailSize), ImVec2(0, 1), ImVec2(1, 0));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("TEXTURE_NAME")) {
            handleTextureDrop(payload, textureID);
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::SameLine();
    ImGui::Text("%s", name);
}

void InspectorPanel::editMaterialComp() {
    if (auto *materialComp = m_SelectionContext.getEntity().tryGetComponent<ZEN::MaterialComp>()) {
        ImGui::SeparatorText("Material");

        const auto &materials = m_AssetLibrary->getAll();
        static ZEN::AssetID selectedMaterialID = materialComp->handle.id();

        ImGui::BeginGroup();
        float buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().FramePadding.x * 2;
        float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
        float comboWidth = ImGui::GetContentRegionAvail().x - buttonWidth - itemSpacing;

        ImGui::SetNextItemWidth(comboWidth);
        if (ImGui::BeginCombo("##material", m_AssetLibrary->getName(selectedMaterialID).c_str())) {
            for (auto &[id, asset]: materials) {
                if (!std::holds_alternative<ZEN::Material>(asset)) continue;

                bool isSelected = (selectedMaterialID == id);
                std::string label = m_AssetLibrary->getName(id);
                if (label.empty()) {
                    label = std::to_string(id);
                }
                if (ImGui::Selectable(label.c_str(), isSelected)) {
                    materialComp->handle.setID(id);
                    selectedMaterialID = id;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::PushID("RemoveMat");
        if (ImGui::Button("X")) {
            if (m_SelectionContext.getEntity().hasComponent<ZEN::MeshDrawableComp>()) {
                materialComp->handle.id() = ZEN::defaultMaterialID;
            } else {
                m_SelectionContext.getEntity().removeComponent<ZEN::MaterialComp>();
            }
        }
        ImGui::PopID();

        ImGui::EndGroup();
    }
}

void InspectorPanel::editBoxColliderComp() {
    if (auto *box = m_SelectionContext.getEntity().tryGetComponent<ZEN::BoxColliderComp>()) {
        ImGui::SeparatorText("Box Collider");

        ImGui::BeginGroup();

        ImGui::DragFloat3("Half Extents",
                          glm::value_ptr(box->halfExtents),
                          0.05f,
                          0.001f,
                          1000.0f
        );

        ImGui::DragFloat3(
            "Offset",
            glm::value_ptr(box->offset),
            0.05f,
            -1000.0f,
            1000.0f
        );

        ImGui::Checkbox("Is Trigger", &box->isTrigger);

        ImGui::SameLine();
        ImGui::PushID("RemoveBoxCollider");
        if (ImGui::Button("X")) {
            m_SelectionContext.getEntity().removeComponent<ZEN::BoxColliderComp>();
        }
        ImGui::PopID();

        ImGui::EndGroup();
    }
}

void InspectorPanel::editSphereColliderComp() {
    if (auto *sphere = m_SelectionContext.getEntity().tryGetComponent<ZEN::SphereColliderComp>()) {
        ImGui::SeparatorText("Sphere Collider");

        ImGui::BeginGroup();

        ImGui::DragFloat("Radius",
                         &sphere->radius,
                         0.05f,
                         0.001f,
                         1000.0f
        );

        ImGui::DragFloat3(
            "Offset",
            glm::value_ptr(sphere->offset),
            0.05f,
            -1000.0f,
            1000.0f
        );

        ImGui::Checkbox("Is Trigger", &sphere->isTrigger);

        ImGui::SameLine();
        ImGui::PushID("RemoveSphereCollider");
        if (ImGui::Button("X")) {
            m_SelectionContext.getEntity().removeComponent<ZEN::SphereColliderComp>();
        }
        ImGui::PopID();

        ImGui::EndGroup();
    }
}

void InspectorPanel::editRigidBodyComp() {
    if (auto *rb = m_SelectionContext.getEntity().tryGetComponent<ZEN::RigidBodyComp>()) {
        ImGui::SeparatorText("Rigid Body");

        ImGui::BeginGroup();

        const char *motionTypes[] = {"Static", "Dynamic", "Kinematic"};
        int motionIndex = 0;

        switch (rb->motionType) {
            case JPH::EMotionType::Static: motionIndex = 0;
                break;
            case JPH::EMotionType::Dynamic: motionIndex = 1;
                break;
            case JPH::EMotionType::Kinematic: motionIndex = 2;
                break;
        }

        if (ImGui::Combo("Motion Type", &motionIndex, motionTypes, IM_ARRAYSIZE(motionTypes))) {
            switch (motionIndex) {
                case 0: rb->motionType = JPH::EMotionType::Static;
                    break;
                case 1: rb->motionType = JPH::EMotionType::Dynamic;
                    break;
                case 2: rb->motionType = JPH::EMotionType::Kinematic;
                    break;
            }
        }

        if (rb->motionType == JPH::EMotionType::Dynamic) {
            ImGui::DragFloat(
                "Mass",
                &rb->mass,
                0.1f,
                0.001f,
                10000.0f
            );
        }

        ImGui::DragFloat(
            "Linear Damping",
            &rb->linearDamping,
            0.01f,
            0.0f,
            10.0f
        );

        ImGui::DragFloat(
            "Angular Damping",
            &rb->angularDamping,
            0.01f,
            0.0f,
            10.0f
        );

        ImGui::Checkbox("Allow Sleep", &rb->allowSleep);

        ImGui::Spacing();
        ImGui::Text("Constraints");

        ImGui::Checkbox("Lock Pos X", &rb->lockPosX);
        ImGui::SameLine();
        ImGui::Checkbox("Lock Pos Y", &rb->lockPosY);
        ImGui::SameLine();
        ImGui::Checkbox("Lock Pos Z", &rb->lockPosZ);

        ImGui::Checkbox("Lock Rot X", &rb->lockRotX);
        ImGui::SameLine();
        ImGui::Checkbox("Lock Rot Y", &rb->lockRotY);
        ImGui::SameLine();
        ImGui::Checkbox("Lock Rot Z", &rb->lockRotZ);

        ImGui::SameLine();
        ImGui::PushID("RemoveRigidBody");
        if (ImGui::Button("X")) {
            m_SelectionContext.getEntity().removeComponent<ZEN::RigidBodyComp>();
        }
        ImGui::PopID();

        ImGui::EndGroup();
    }
}


void InspectorPanel::editMaterialProps() {
    ImGui::SeparatorText("Material");

    ImGui::DragFloat3("Albedo", &m_SelectionContext.getMaterial()->albedo.x, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Metallic", &m_SelectionContext.getMaterial()->metallic, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Roughness", &m_SelectionContext.getMaterial()->roughness, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Ambient Oclussion", &m_SelectionContext.getMaterial()->ao, 0.01f, 0.0f, 1.0f);
    ImGui::Checkbox("Is Metal", &m_SelectionContext.getMaterial()->metal);

    if (ImGui::TreeNode("Shader")) {
        if (auto shaderComp = m_SelectionContext.getMaterial()->shader) {
            ImGui::SeparatorText("Shader");

            const auto &assets = m_AssetLibrary->getAll();
            static ZEN::AssetID selectedShaderID = shaderComp;

            if (selectedShaderID != shaderComp) {
                selectedShaderID = shaderComp;
            }

            ImGui::BeginGroup();
            float buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().FramePadding.x * 2;
            float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
            float comboWidth = ImGui::GetContentRegionAvail().x - buttonWidth - itemSpacing;

            ImGui::SetNextItemWidth(comboWidth);
            if (ImGui::BeginCombo("##shader", m_AssetLibrary->getName(selectedShaderID).c_str())) {
                for (auto &[id, asset]: assets) {
                    if (!std::holds_alternative<ZEN::ShaderData>(asset)) continue;

                    bool isSelected = (selectedShaderID == id);
                    std::string label = m_AssetLibrary->getName(id);
                    if (label.empty()) {
                        label = std::to_string(id);
                    }
                    if (ImGui::Selectable(label.c_str(), isSelected)) {
                        m_SelectionContext.getMaterial()->shader = id;
                        selectedShaderID = id;
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::EndGroup();
        }
        ImGui::TreePop();
    }


    if (ImGui::TreeNode("Texture")) {
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 150);
        ImGui::SetColumnWidth(1, 100);

        renderTextureDrop(m_SelectionContext.getMaterial()->texture, "Albedo");
        ImGui::NextColumn();
        ImGui::Checkbox("Use##Albedo", &m_SelectionContext.getMaterial()->useAlbedo);
        ImGui::NextColumn();

        renderTextureDrop(m_SelectionContext.getMaterial()->metallicTex, "Metallic");
        ImGui::NextColumn();
        ImGui::Checkbox("Use##Metallic", &m_SelectionContext.getMaterial()->useMetallic);
        ImGui::NextColumn();

        renderTextureDrop(m_SelectionContext.getMaterial()->roughnessTex, "Roughness");
        ImGui::NextColumn();
        ImGui::Checkbox("Use##Roughness", &m_SelectionContext.getMaterial()->useRoughness);
        ImGui::NextColumn();

        renderTextureDrop(m_SelectionContext.getMaterial()->aoTex, "AO");
        ImGui::NextColumn();
        ImGui::Checkbox("Use##AO", &m_SelectionContext.getMaterial()->useAO);
        ImGui::NextColumn();

        renderTextureDrop(m_SelectionContext.getMaterial()->normalTex, "Normal");
        ImGui::NextColumn();
        ImGui::Checkbox("Use##Normal", &m_SelectionContext.getMaterial()->useNormal);
        ImGui::NextColumn();

        ImGui::Columns(1);
        ImGui::TreePop();
    }
}

void InspectorPanel::inspectEntity() {
    ImGui::Text(
        "%s", std::format("Scene Entity ID: {}", (uint32_t) ((entt::entity) (m_SelectionContext.getEntity()))).c_str());
    if (auto name = m_SelectionContext.getEntity().tryGetComponent<ZEN::TagComp>()) {
        char buffer[128];
        strncpy(buffer, name->tag.c_str(), sizeof(buffer));
        if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
            name->tag = buffer;
        }
    }

    if (auto *transform = m_SelectionContext.getEntity().tryGetComponent<ZEN::TransformComp>()) {
        inspectTransform(*transform);
    }

    if (auto *cameraComp = m_SelectionContext.getEntity().tryGetComponent<ZEN::SceneCameraComp>()) {
        ImGui::SeparatorText("Scene Camera");

        ImGui::DragFloat("Aspect", &cameraComp->aspect, 0.01f, 0.0f, 2.0f);
        ImGui::DragFloat("FOV", &cameraComp->fov, 0.01f, 0.0f, 2.0f);
        ImGui::DragFloat("Near", &cameraComp->near, 0.01f, 0.0f, 2.0f);
        ImGui::DragFloat("Far", &cameraComp->far, 0.1f, 0.0f, 1000.0f);
        //ImGui::Checkbox("IsPrimary", &cameraComp->isPrimary);
    }
    else if (auto *cameraComp = m_SelectionContext.getEntity().tryGetComponent<ZEN::CameraComp>()) {
        ImGui::SeparatorText("Camera");

        ImGui::DragFloat("Aspect", &cameraComp->aspect, 0.01f, 0.0f, 2.0f);
        ImGui::DragFloat("FOV", &cameraComp->fov, 0.01f, 0.0f, 2.0f);
        ImGui::DragFloat("Near", &cameraComp->near, 0.01f, 0.0f, 2.0f);
        ImGui::DragFloat("Far", &cameraComp->far, 0.1f, 0.0f, 1000.0f);
        ImGui::Checkbox("IsPrimary", &cameraComp->isPrimary);
    }

    editComponents();


    ImGui::Separator();


}

void InspectorPanel::inspectMaterial() {
    editMaterialProps();
}

void InspectorPanel::onUIRender() {
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
        //m_Engine->getDispatcher().trigger<ZEN::PanelFocusEvent>(
        //    ZEN::PanelFocusEvent{.panel = "Inspector"}
        //);
    }
    if (m_SelectionContext.getEntity().isValid()) {
        inspectEntity();
    } else if (m_SelectionContext.getMaterial()) {
        inspectMaterial();
    }

    ImGui::End();
}

void InspectorPanel::onEvent(ZEN::Event &event) {
    ZEN::EventDispatcher dispatcher(event);

    dispatcher.dispatch<ZEN::RunPlayModeEvent>([this](ZEN::RunPlayModeEvent &e) { return onPlayModeEvent(e); });
}

bool InspectorPanel::onPlayModeEvent(ZEN::RunPlayModeEvent &e) {
    if (e.getPlaying()) {
        //ZEN::Application::get().popOverlay(this);
    }
    return false;
}

void InspectorPanel::handleMaterialDrop(const ImGuiPayload *payload) {
    ZEN::AssetID droppedID = *(ZEN::AssetID *) payload->Data;

    if (!m_SelectionContext.getEntity().hasComponent<ZEN::MaterialComp>()) {
        m_SelectionContext.getEntity().addComponent<ZEN::MaterialComp>(ZEN::AssetHandle<ZEN::Material>(droppedID));
    } else {
        m_SelectionContext.getEntity().getComponent<ZEN::MaterialComp>().handle.setID(droppedID);
    }
}


void InspectorPanel::handleMeshDrop(const ImGuiPayload *payload) {
    ZEN::AssetID droppedID = *(ZEN::AssetID *) payload->Data;

    if (!m_SelectionContext.getEntity().hasComponent<ZEN::MeshComp>()) {
        m_SelectionContext.getEntity().addComponent<ZEN::MeshComp>(ZEN::AssetHandle<ZEN::MeshData>(droppedID));
    } else {
        auto &meshComp = m_SelectionContext.getEntity().getComponent<ZEN::MeshComp>();
        meshComp.handle.setID(droppedID);

        if (m_SelectionContext.getEntity().hasComponent<ZEN::MeshDrawableComp>())
            m_SelectionContext.getEntity().removeComponent<ZEN::MeshDrawableComp>();
    }
}
