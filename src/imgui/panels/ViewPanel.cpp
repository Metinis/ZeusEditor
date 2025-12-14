#include "ViewPanel.h"

ViewPanel::ViewPanel(ZEN::ZEngine* engine, SelectionContext& selection) : m_Engine(engine), m_SelectionContext(selection) {
    m_PanelSize = {800, 600};
    //m_Engine->getDispatcher().attach<ZEN::ToggleEditorEvent, ViewPanel, &ViewPanel::onToggleEditor>(this);
}
/*void ViewPanel::onToggleEditor(ZEN::ToggleEditorEvent &e) {
    //ZEN::Application::get().popOverlay(this);
    m_EditorToggled = false;
}*/
void ViewPanel::onUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    float menuBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

    if(m_EditorToggled) {
        ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.2f, menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.6f, displaySize.y * 0.7f - menuBarHeight));
    }
    else {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y));
    }


    ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse);


    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
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

    if (panelAspect > aspect)
    {
        finalSize.y = m_PanelSize.y;
        finalSize.x = finalSize.y * aspect;
    }
    else
    {
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
        (void*)(intptr_t)m_Engine->getRenderer().getColorTextureHandle(),
        finalSize,
        ImVec2(0, 1), // uv0 (top-left)
        ImVec2(1, 0)  // uv1 (bottom-right)
    );
    ImVec2 imagePos = ImGui::GetItemRectMin(); // top-left corner of the last item
    ImVec2 mousePos = ImGui::GetMousePos();
    if(ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_NAME")) {
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
            m_Engine->getScene().createEntity().addComponent<ZEN::MeshComp>(ZEN::AssetHandle<ZEN::MeshData>(assetID));
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void ViewPanel::onEvent(ZEN::Event &event) {
    ZEN::EventDispatcher dispatcher(event);

    dispatcher.dispatch<ZEN::RunPlayModeEvent>([this](ZEN::RunPlayModeEvent& e) {return onPlayModeEvent(e); });
}

bool ViewPanel::onPlayModeEvent(ZEN::RunPlayModeEvent &e) {
    if(e.getPlaying()) {
        ZEN::Application::get().popOverlay(this);
    }
    return false;
}

