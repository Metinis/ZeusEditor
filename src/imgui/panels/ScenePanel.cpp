
#include "ScenePanel.h"

#include <imgui.h>
#include <ZeusEngineCore/Components.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/ModelLibrary.h>

ScenePanel::ScenePanel(entt::dispatcher &dispatcher) {
    dispatcher.sink<ZEN::SelectEntityEvent>().connect<&ScenePanel::onEntitySelect>(*this);
}
void ScenePanel::drawEntityNode(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity entity) {
    auto &name = registry.get<ZEN::TagComp>(entity);

    ImGuiTreeNodeFlags flags =
        (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    // Determine if this entity has children
    bool hasChildren = false;
    registry.view<ZEN::ParentComp>().each([&](auto childEntity, auto &pc){
        if (pc.parent == entity) {
            hasChildren = true;
        }
    });
    if (!hasChildren) flags |= ImGuiTreeNodeFlags_Leaf;

    const bool opened = ImGui::TreeNodeEx((void *) (intptr_t) entity, flags, "%s", name.tag.c_str());

    if (ImGui::IsItemClicked())
        dispatcher.trigger<ZEN::SelectEntityEvent>({entity});

    if (opened) {
        registry.view<ZEN::ParentComp>().each([&](auto childEntity, auto &pc){
            if (pc.parent == entity) {
                drawEntityNode(registry, dispatcher, childEntity);
            }
        });
        ImGui::TreePop();
    }
}
void ScenePanel::onImGuiRender(entt::dispatcher& dispatcher, entt::registry& registry) {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f));

    ImGui::Begin("Scene Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        dispatcher.trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Scene" }
        );
    }
    auto view = registry.view<ZEN::TagComp>();

    if (ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Add Empty Entity")) {
            entt::entity entity = registry.create();
            registry.emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Empty Entity"});
            dispatcher.trigger<ZEN::SelectEntityEvent>({entity});
        }
        if (ImGui::MenuItem("Add Cube")) {
            entt::entity entity = registry.create();
            registry.emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Cube"});
            dispatcher.trigger<ZEN::SelectEntityEvent>({entity});

            registry.emplace<ZEN::TransformComp>(entity);
            registry.emplace<ZEN::MeshComp>(entity, *ZEN::ModelLibrary::get("Cube"));
        }
        if (ImGui::MenuItem("Add Sphere")) {
            entt::entity entity = registry.create();
            registry.emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Sphere"});
            dispatcher.trigger<ZEN::SelectEntityEvent>({entity});

            registry.emplace<ZEN::TransformComp>(entity);
            registry.emplace<ZEN::MeshComp>(entity, *ZEN::ModelLibrary::get("Sphere"));
        }
        ImGui::EndPopup();
    }
    for(auto entity : view) {
        if (!registry.any_of<ZEN::ParentComp>(entity) ||
        registry.get<ZEN::ParentComp>(entity).parent == entt::null) {
            drawEntityNode(registry, dispatcher, entity);
        }
    }



    ImGui::End();
}

void ScenePanel::onEntitySelect(ZEN::SelectEntityEvent &e) {
    m_SelectedEntity = e.entity;
}
