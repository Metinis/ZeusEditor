
#include "ScenePanel.h"
#include <imgui.h>
#include <ZeusEngineCore/Components.h>
#include <ZeusEngineCore/Entity.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/ModelLibrary.h>
#include <ZeusEngineCore/Scene.h>
#include <ZeusEngineCore/ZEngine.h>
#include <ZeusEngineCore/EventDispatcher.h>

ScenePanel::ScenePanel(ZEN::ZEngine* engine) : m_Engine(engine) {
    m_Engine->getDispatcher().attach<ZEN::SelectEntityEvent, ScenePanel, &ScenePanel::onEntitySelect>(this);
}
void ScenePanel::drawEntityNode(ZEN::Entity& entity) {
    auto &name = entity.getComponent<ZEN::TagComp>();

    ImGuiTreeNodeFlags flags =
        (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    bool hasChildren = false;
    for (auto childEntity : m_Engine->getScene().getEntities<ZEN::ParentComp>()) {
        auto& pc = childEntity.getComponent<ZEN::ParentComp>();
        if (pc.parent == entity) {
            hasChildren = true;
        }
    }

    if (!hasChildren) flags |= ImGuiTreeNodeFlags_Leaf;

    const bool opened = ImGui::TreeNodeEx((void *) (intptr_t)entity, flags, "%s", name.tag.c_str());

    ImGui::PushID((intptr_t)entity);
    if (ImGui::BeginPopupContextItem("EntityPanelContext", ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Delete")) {
            m_Engine->getScene().removeEntity(entity);

        }
        ImGui::EndPopup();
    }
    ImGui::PopID();

    if (ImGui::IsItemClicked())
        m_Engine->getDispatcher().trigger<ZEN::SelectEntityEvent>({entity});

    if (opened) {
        auto entities = m_Engine->getScene().getEntities<ZEN::ParentComp>();
        for(auto e : entities) {
            auto parentComp = e.getComponent<ZEN::ParentComp>();
            if(parentComp.parent == entity) {
                drawEntityNode(e);
            }
        }
        ImGui::TreePop();
    }
}
void ScenePanel::onUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    float menuBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f - menuBarHeight));

    ImGui::Begin("Scene Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        m_Engine->getDispatcher().trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Scene" }
        );
    }
    auto view = m_Engine->getScene().getEntities<ZEN::TagComp>();

    if (ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Add Empty Entity")) {
            ZEN::Entity entity = m_Engine->getScene().createEntity();
            m_Engine->getDispatcher().trigger<ZEN::SelectEntityEvent>({.entity = entity});
        }
        if (ImGui::MenuItem("Add Cube")) {
            ZEN::Entity entity = m_Engine->getScene().createEntity("Cube");
            entity.addComponent<ZEN::MeshComp>(ZEN::MeshComp{"Cube"});
            m_Engine->getDispatcher().trigger<ZEN::SelectEntityEvent>({.entity = entity});
        }
        if (ImGui::MenuItem("Add Sphere")) {
            ZEN::Entity entity = m_Engine->getScene().createEntity("Sphere");
            entity.addComponent<ZEN::MeshComp>(ZEN::MeshComp{"Sphere"});
            m_Engine->getDispatcher().trigger<ZEN::SelectEntityEvent>({.entity = entity});
        }
        ImGui::EndPopup();
    }
    for(auto entity : view) {
        if (!entity.hasComponent<ZEN::ParentComp>() || !entity.getComponent<ZEN::ParentComp>().parent.isValid()) {
            drawEntityNode(entity);
        }
    }



    ImGui::End();
}

void ScenePanel::onEntitySelect(ZEN::SelectEntityEvent &e) {
    m_SelectedEntity = e.entity;
}
