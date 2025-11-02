
#include "ViewPanel.h"
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/Scene.h>
#include <ZeusEngineCore/ZEngine.h>
#include <ZeusEngineCore/EventDispatcher.h>
#include <ZeusEngineCore/ModelLibrary.h>

ViewPanel::ViewPanel(ZEN::ZEngine* engine) : m_Engine(engine){
    m_PanelSize = {800, 600};
}

void ViewPanel::onImGuiRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    float menuBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

    ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.2f, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.6f, displaySize.y * 0.7f - menuBarHeight));

    ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse);


    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // make panel focused, same as left-click
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        m_Engine->getDispatcher().trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{
                .panel = "Scene View"
            }
        );
    }

    ImVec2 newSize = ImGui::GetContentRegionAvail();
    if (m_PanelSize.x != newSize.x || m_PanelSize.y != newSize.y) {
        m_PanelSize = newSize;
        m_Engine->getDispatcher().trigger<ZEN::SceneViewResizeEvent>(
            ZEN::SceneViewResizeEvent{ newSize.x, newSize.y }
        );
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