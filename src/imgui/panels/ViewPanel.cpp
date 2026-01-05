#include "ViewPanel.h"
#include <ImGuizmo/ImGuizmo.h>

#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"
#include "ZeusEngineCore/input/KeyCodes.h"

ViewPanel::ViewPanel(ZEN::ZEngine *engine, SelectionContext &selection) : m_Engine(engine),
                                                                          m_SelectionContext(selection) {
    m_PanelSize = {800, 600};
    //m_Engine->getDispatcher().attach<ZEN::ToggleEditorEvent, ViewPanel, &ViewPanel::onToggleEditor>(this);
}

/*void ViewPanel::onToggleEditor(ZEN::ToggleEditorEvent &e) {
    //ZEN::Application::get().popOverlay(this);
    m_EditorToggled = false;
}*/
void ViewPanel::onUIRender() {
    ImGuiIO &io = ImGui::GetIO();

    m_ImGuiWantsMouse = io.WantCaptureMouse;
    m_ImGuiWantsKeyboard = io.WantCaptureKeyboard;
    ImVec2 displaySize = io.DisplaySize;
    float menuBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

    if (m_EditorToggled) {
        ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.2f, menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.6f, displaySize.y * 0.7f - menuBarHeight));
    } else {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y));
    }


    ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
                                        | ImGuiWindowFlags_NoCollapse);

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus();
    }

    ImVec2 newSize = ImGui::GetContentRegionAvail();
    if (m_PanelSize.x != newSize.x || m_PanelSize.y != newSize.y) {
        m_PanelSize = newSize;
    }
    glm::vec2 rendererSize = m_Engine->getRenderer().getSize();
    ImVec2 texSize(rendererSize.x, rendererSize.y);
    float aspect = texSize.x / texSize.y;
    ImVec2 finalSize;
    float panelAspect = m_PanelSize.x / m_PanelSize.y;

    if (panelAspect > aspect) {
        finalSize.y = m_PanelSize.y;
        finalSize.x = finalSize.y * aspect;
    } else {
        finalSize.x = m_PanelSize.x;
        finalSize.y = finalSize.x / aspect;
    }
    float offsetX = (m_PanelSize.x - finalSize.x) * 0.5f;
    float offsetY = (m_PanelSize.y - finalSize.y) * 0.5f;

    ImGui::SetCursorPos(ImVec2(
        ImGui::GetCursorPos().x + offsetX,
        ImGui::GetCursorPos().y + offsetY
    ));

    ImGui::Image(
        (void *) (intptr_t) m_Engine->getRenderer().getColorTextureHandle(),
        finalSize,
        ImVec2(0, 1), // uv0 (top-left)
        ImVec2(1, 0) // uv1 (bottom-right)
    );

    drawGizmo();

    m_IsViewportHovered = ImGui::IsItemHovered();
    m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

    ImVec2 imagePos = ImGui::GetItemRectMin(); // top-left corner of the last item
    ImVec2 mousePos = ImGui::GetMousePos();
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("MESH_NAME")) {
            ZEN::AssetID assetID;
            if (payload->DataSize == sizeof(ZEN::AssetID)) {
                std::memcpy(&assetID, payload->Data, sizeof(ZEN::AssetID));
            }
            // Mouse position relative to viewport
            ImVec2 relative = {
                mousePos.x - imagePos.x,
                mousePos.y - imagePos.y
            };

            std::cout << "Dropped payload " << assetID
                    << " at viewport coords: "
                    << relative.x << ", " << relative.y << "\n";
            if (ZEN::Project::getActive()->getAssetLibrary()->get<ZEN::MeshData>(assetID)) {
                m_Engine->getScene().createEntity().addComponent<ZEN::MeshComp>(
                    ZEN::AssetHandle<ZEN::MeshData>(assetID));
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void ViewPanel::onEvent(ZEN::Event &event) {
    ZEN::EventDispatcher dispatcher(event);

    dispatcher.dispatch<ZEN::RunPlayModeEvent>([this](ZEN::RunPlayModeEvent &e) { return onPlayModeEvent(e); });
    dispatcher.dispatch<ZEN::KeyPressedEvent>([this](ZEN::KeyPressedEvent &e) { return onKeyPressedEvent(e); });
    dispatcher.dispatch<ZEN::MouseButtonPressedEvent>([this](ZEN::MouseButtonPressedEvent &e) {
        return onMouseButtonPressedEvent(e);
    });
    dispatcher.dispatch<ZEN::MouseButtonReleasedEvent>([this](ZEN::MouseButtonReleasedEvent &e) {
        return onMouseButtonReleasedEvent(e);
    });
    dispatcher.dispatch<ZEN::MouseMovedEvent>([this](ZEN::MouseMovedEvent &e) { return onMouseMovedEvent(e); });
}

void ViewPanel::drawGizmo() {
    auto selection = m_SelectionContext.getEntity();
    if (!selection.isValid()) {
        return;
    }
    ImVec2 imageMin = ImGui::GetItemRectMin();
    ImVec2 imageMax = ImGui::GetItemRectMax();
    ImVec2 imageSize(imageMax.x - imageMin.x, imageMax.y - imageMin.y);

    if (imageSize.x <= 0 || imageSize.y <= 0) {
        std::cout << "ERROR: Invalid image size!" << std::endl;
        return;
    }

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(imageMin.x, imageMin.y, imageSize.x, imageSize.y);

    auto cameraEntity = m_Engine->getScene().getSceneCamera();
    glm::mat4 view = cameraEntity.getComponent<ZEN::TransformComp>().getViewMatrix();
    glm::mat4 proj = cameraEntity.getComponent<ZEN::SceneCameraComp>().projection;

    auto& tc = selection.getComponent<ZEN::TransformComp>();
    glm::mat4 worldMatrix = tc.worldMatrix;
    bool manipulated = ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(proj),
        m_GizmoType,
        m_GizmoMode,
        glm::value_ptr(worldMatrix)
    );

    if (ImGuizmo::IsUsing()) {
        m_ImGuiWantsMouse = true;
        glm::mat4 localMatrix = worldMatrix;

        if (selection.hasComponent<ZEN::ParentComp>()) {
            auto parent = m_Engine->getScene().getEntity(selection.getComponent<ZEN::ParentComp>().parentID);
            auto& parentTC = parent.getComponent<ZEN::TransformComp>();
            localMatrix = glm::inverse(parentTC.worldMatrix) * worldMatrix;
        }

        ZEN::TransformComp::decomposeTransform(localMatrix, tc.localPosition, tc.localRotation, tc.localScale);
    }
}

bool ViewPanel::onPlayModeEvent(ZEN::RunPlayModeEvent &e) {
    if (e.getPlaying()) {
        ZEN::Application::get().popOverlay(this);
    }
    return false;
}

bool ViewPanel::onKeyPressedEvent(ZEN::KeyPressedEvent &e) {
    //this blocks input if we are not focused on this panel
    if (e.getKeyCode() == ZEN::Key::R) {
        m_GizmoType = ImGuizmo::ROTATE;
    }
    else if (e.getKeyCode() == ZEN::Key::T){
        m_GizmoType = ImGuizmo::TRANSLATE;
    }
    else if (e.getKeyCode() == ZEN::Key::Y){
        m_GizmoType = ImGuizmo::SCALE;
    }
    else if (e.getKeyCode() == ZEN::Key::Q) {
        m_GizmoMode = ImGuizmo::WORLD;
    }
    else if (e.getKeyCode() == ZEN::Key::E) {
        m_GizmoMode = ImGuizmo::LOCAL;
    }
    if (m_ImGuiWantsKeyboard)
        return true;
    if (!m_IsFocused) {
        return true;
    }
    return false;
}

bool ViewPanel::onMouseButtonPressedEvent(ZEN::MouseButtonPressedEvent &e) {
    if (m_ImGuiWantsMouse && !m_IsViewportHovered)
        return true;

    if (!m_IsViewportHovered)
        return true;
    return false;
}

bool ViewPanel::onMouseButtonReleasedEvent(ZEN::MouseButtonReleasedEvent &e) {
    return false;
}

bool ViewPanel::onMouseMovedEvent(ZEN::MouseMovedEvent &e) {
    if (!m_IsFocused)
        return true;
    return false;
}
