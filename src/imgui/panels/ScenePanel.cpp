
#include "ScenePanel.h"
#include <imgui.h>
#include <ZeusEngineCore/Components.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/ModelLibrary.h>
#include <ZeusEngineCore/Scene.h>

ScenePanel::ScenePanel(ZEN::Scene* scene, ZEN::ModelLibrary* modelLibrary) : m_Scene(scene), m_ModelLibrary(modelLibrary){
    m_Scene->getDispatcher().sink<ZEN::SelectEntityEvent>().connect<&ScenePanel::onEntitySelect>(*this);
}
void ScenePanel::drawEntityNode(entt::entity entity) {
    auto &name = m_Scene->getRegistry().get<ZEN::TagComp>(entity);

    ImGuiTreeNodeFlags flags =
        (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    // Determine if this entity has children
    bool hasChildren = false;
    m_Scene->getRegistry().view<ZEN::ParentComp>().each([&](auto childEntity, auto &pc){
        if (pc.parent == entity) {
            hasChildren = true;
        }
    });
    if (!hasChildren) flags |= ImGuiTreeNodeFlags_Leaf;

    const bool opened = ImGui::TreeNodeEx((void *) (intptr_t) entity, flags, "%s", name.tag.c_str());

    if (ImGui::IsItemClicked())
        m_Scene->getDispatcher().trigger<ZEN::SelectEntityEvent>({entity});

    if (opened) {
        m_Scene->getRegistry().view<ZEN::ParentComp>().each([&](auto childEntity, auto &pc){
            if (pc.parent == entity) {
                drawEntityNode(childEntity);
            }
        });
        ImGui::TreePop();
    }
}
void ScenePanel::onImGuiRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f));

    ImGui::Begin("Scene Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        m_Scene->getDispatcher().trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Scene" }
        );
    }
    auto view = m_Scene->getRegistry().view<ZEN::TagComp>();

    if (ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Add Empty Entity")) {
            entt::entity entity = m_Scene->getRegistry().create();
            m_Scene->getRegistry().emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Empty Entity"});
            m_Scene->getDispatcher().trigger<ZEN::SelectEntityEvent>({entity});
        }
        if (ImGui::MenuItem("Add Cube")) {
            entt::entity entity = m_Scene->createEntity();
            m_Scene->getRegistry().emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Cube"});
            m_Scene->getDispatcher().trigger<ZEN::SelectEntityEvent>({entity});

            m_Scene->getRegistry().emplace<ZEN::TransformComp>(entity);
            m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity, *m_ModelLibrary->getMesh("Cube"));
        }
        if (ImGui::MenuItem("Add Sphere")) {
            entt::entity entity = m_Scene->createEntity();
            m_Scene->getRegistry().emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Sphere"});
            m_Scene->getDispatcher().trigger<ZEN::SelectEntityEvent>({entity});

            m_Scene->getRegistry().emplace<ZEN::TransformComp>(entity);
            m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity, *m_ModelLibrary->getMesh("Sphere"));
        }
        ImGui::EndPopup();
    }
    for(auto entity : view) {
        if (!m_Scene->getRegistry().any_of<ZEN::ParentComp>(entity) ||
        m_Scene->getRegistry().get<ZEN::ParentComp>(entity).parent == entt::null) {
            drawEntityNode(entity);
        }
    }



    ImGui::End();
}

void ScenePanel::onEntitySelect(ZEN::SelectEntityEvent &e) {
    m_SelectedEntity = e.entity;
}
