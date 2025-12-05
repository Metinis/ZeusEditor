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
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        //m_Engine->getDispatcher().trigger<ZEN::PanelFocusEvent>(
        //    ZEN::PanelFocusEvent{
        //        .panel = "Scene View"
        //    }
        //);
    }

    ImVec2 newSize = ImGui::GetContentRegionAvail();
    if (m_PanelSize.x != newSize.x || m_PanelSize.y != newSize.y) {
        m_PanelSize = newSize;
        ZEN::Application::get().setViewportSize({newSize.x, newSize.y});
        //m_Engine->getDispatcher().trigger<ZEN::SceneViewResizeEvent>(
        //    ZEN::SceneViewResizeEvent{ newSize.x, newSize.y }
        //);
    }
    ImGui::Image(
        (void*)(intptr_t)m_Engine->getRenderer().getColorTextureHandle(),
        m_PanelSize,
        ImVec2(0, 1), // uv0 (top-left)
        ImVec2(1, 0)  // uv1 (bottom-right)
    );
    ImVec2 imagePos = ImGui::GetItemRectMin(); // top-left corner of the last item
    ImVec2 mousePos = ImGui::GetMousePos();
    if(ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_NAME")) {
            const char* data = (const char*)payload->Data;
            //auto mesh = m_Engine->getModelLibrary().getMesh(data);
            // Mouse position relative to viewport
            ImVec2 relative = {
                mousePos.x - imagePos.x,
                mousePos.y - imagePos.y
            };

            std::cout << "Dropped payload " << data
                      << " at viewport coords: "
                      << relative.x << ", " << relative.y << "\n";
            m_Engine->getScene().createEntity().addComponent<ZEN::MeshComp>(ZEN::MeshComp{.name = data});
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
    else {
        ZEN::Application::get().pushOverlay(this);
    }
    return false;
}

