#include "ScenePanel.h"

#include "imgui_internal.h"

ScenePanel::ScenePanel(ZEN::ZEngine* engine, SelectionContext& selection) : m_Engine(engine), m_SelectionContext(selection)  {

}
static constexpr const char* ENTITY_DRAG_PAYLOAD = "ZEN_ENTITY";

void ScenePanel::drawEntityNode(ZEN::Entity& entity) {

    auto &name = entity.getComponent<ZEN::TagComp>();

    ImGuiTreeNodeFlags flags =
        (m_SelectionContext.getEntity() == entity ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    bool hasChildren = false;
    for (auto childEntity : m_Engine->getScene().getEntities<ZEN::ParentComp>()) {
        auto& pc = childEntity.getComponent<ZEN::ParentComp>();
        if (pc.parentID == entity.getComponent<ZEN::UUIDComp>().uuid) {
            hasChildren = true;
        }
    }

    if (!hasChildren) flags |= ImGuiTreeNodeFlags_Leaf;

    const bool opened = ImGui::TreeNodeEx((void *) (intptr_t)entity, flags, "%s", name.tag.c_str());

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        ZEN::Entity dragged = entity;
        ImGui::SetDragDropPayload(ENTITY_DRAG_PAYLOAD, &dragged.getComponent<ZEN::UUIDComp>().uuid,
                sizeof(dragged.getComponent<ZEN::UUIDComp>().uuid));
        ImGui::Text("Move %s", name.tag.c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENTITY_DRAG_PAYLOAD)) {

            ZEN::UUID droppedID = *(const ZEN::UUID*)payload->Data;
            ZEN::UUID targetID  = entity.getComponent<ZEN::UUIDComp>().uuid;

            if (droppedID != targetID) {
                m_PendingReparent = { droppedID, targetID };
            }
        }

        ImGui::EndDragDropTarget();

    }



    ImGui::PushID((intptr_t)entity);
    if (ImGui::BeginPopupContextItem("EntityPanelContext", ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Delete")) {
            m_Engine->getScene().removeEntity(entity);

        }
        ImGui::EndPopup();
    }
    ImGui::PopID();

    if (ImGui::IsItemClicked()) {
        m_SelectionContext.setEntity(entity);
    }

    if (opened) {
        auto entities = m_Engine->getScene().getEntities<ZEN::ParentComp>();
        for(auto e : entities) {
            auto parentComp = e.getComponent<ZEN::ParentComp>();
            if(parentComp.parentID == entity.getComponent<ZEN::UUIDComp>().uuid) {
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
    if (ImGui::BeginDragDropTargetCustom(
            ImGui::GetCurrentWindow()->Rect(),
            ImGui::GetID("SceneRootDrop")))
    {
        if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload(ENTITY_DRAG_PAYLOAD))
        {
            ZEN::UUID id = *(const ZEN::UUID*)payload->Data;
            auto child = m_Engine->getScene().getEntity(id);

            if (child.isValid() && child.hasComponent<ZEN::ParentComp>())
            {
                child.removeParent();
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus();
    }
    auto view = m_Engine->getScene().getEntities<ZEN::TagComp>();

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
    ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("SceneContextMenu");
    }

    if (ImGui::BeginPopup("SceneContextMenu"))
    {
        if (ImGui::MenuItem("Add Empty Entity")) {
            ZEN::Entity entity = m_Engine->getScene().createEntity();
            m_SelectionContext.setEntity(entity);


        }
        if (ImGui::MenuItem("Add Cube")) {
            ZEN::Entity entity = m_Engine->getScene().createEntity("Cube");
            auto mesh = ZEN::AssetHandle<ZEN::MeshData>(ZEN::defaultCubeID);
            entity.addComponent<ZEN::MeshComp>(mesh);
            m_SelectionContext.setEntity(entity);


        }
        if (ImGui::MenuItem("Add Sphere")) {
            ZEN::Entity entity = m_Engine->getScene().createEntity("Sphere");
            auto mesh = ZEN::AssetHandle<ZEN::MeshData>(ZEN::defaultSphereID);
            entity.addComponent<ZEN::MeshComp>(mesh);
            m_SelectionContext.setEntity(entity);


        }
        ImGui::EndPopup();
    }
    for(auto entity : view) {
        if (!entity.hasComponent<ZEN::ParentComp>()) {
            drawEntityNode(entity);
        }
    }

    ImGui::End();
    if (m_PendingReparent) {
        auto& [childID, parentID] = *m_PendingReparent;

        auto child  = m_Engine->getScene().getEntity(childID);
        auto parent = m_Engine->getScene().getEntity(parentID);

        if (child.isValid() && parent.isValid() &&
            !m_Engine->getScene().isDescendantOf(child, parent))
        {
            auto& pc = child.hasComponent<ZEN::ParentComp>()
                ? child.getComponent<ZEN::ParentComp>()
                : child.addParent(parentID);

            //pc.parentID = parentID;
        }

        m_PendingReparent.reset();
    }

}

void ScenePanel::onEvent(ZEN::Event &event) {
    ZEN::EventDispatcher dispatcher(event);

    dispatcher.dispatch<ZEN::RunPlayModeEvent>([this](ZEN::RunPlayModeEvent& e) {return onPlayModeEvent(e); });
}

bool ScenePanel::onPlayModeEvent(ZEN::RunPlayModeEvent &e) {
    if(e.getPlaying()) {
        //ZEN::Application::get().popOverlay(this);
    }
    return false;
}