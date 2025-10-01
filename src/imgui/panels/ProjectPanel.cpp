
#include "ProjectPanel.h"
#include <imgui.h>
#include <ZeusEngineCore/InputEvents.h>
#include <ZeusEngineCore/ModelLibrary.h>
#include <ZeusEngineCore/ZEngine.h>
#include <ZeusEngineCore/EventDispatcher.h>


ProjectPanel::ProjectPanel(ZEN::ZEngine* engine) : m_Engine(engine){

}
void checkWindowFocus(ZEN::EventDispatcher& dispatcher) {
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus();
    }
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        dispatcher.trigger<ZEN::PanelFocusEvent>(
            ZEN::PanelFocusEvent{ .panel = "Project"}
        );
    }
}
void drawSearchBar() {
    static char search[128] = "";
    ImGui::InputTextWithHint("##search", "Search assets...", search, sizeof(search));
    ImGui::Separator();
}
void ProjectPanel::drawFolderTree() {
    ImGui::BeginChild("LeftPane", ImVec2(200, 0), true);

    if(ImGui::TreeNode("Assets")) {
        if (ImGui::Selectable("Meshes", m_SelectedFolder == "Meshes")) {
            m_SelectedFolder = "Meshes";
        }
        if (ImGui::Selectable("Materials", m_SelectedFolder == "Materials")) {
            m_SelectedFolder = "Materials";
        }
        if (ImGui::Selectable("Scenes", m_SelectedFolder == "Scenes")) {
            m_SelectedFolder = "Scenes";
        }

        ImGui::TreePop();
    }

    ImGui::EndChild();

}
void ProjectPanel::drawAssetGrid() {
    ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
    const float thumbnailSize = 64.0f;
    const float padding = 16.0f;
    const float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1) columnCount = 1;

    ImGui::Columns(columnCount, 0, false);
    if(m_SelectedFolder == "Meshes") {
        for(auto& [name, mesh] : m_Engine->getModelLibrary().getAllMeshes()) {
            ImGui::PushID(name.c_str());
            ImGui::Button("##thumbnail", ImVec2(thumbnailSize, thumbnailSize));
            if(ImGui::BeginDragDropSource()) {
                //handle drop material onto object
                const char* payload = name.c_str();
                ImGui::SetDragDropPayload("MESH_NAME", payload, strlen(payload) + 1);

                //ImGui::Image(texHandle, ImVec2(16, 16));
                ImGui::Text(payload);
                ImGui::EndDragDropSource();
            }
            ImGui::TextWrapped("%s", name.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }
    }
    if(m_SelectedFolder == "Materials") {
        for(auto& [name, material] : m_Engine->getModelLibrary().getAllMaterials()) {
            ImGui::PushID(name.c_str());
            void* texHandle = (void*)m_Engine->getRenderer().getResourceManager()->getTexture(material->textureIDs[0]);
            ImGui::ImageButton("##thumbnail", texHandle, ImVec2(thumbnailSize, thumbnailSize));

            if(ImGui::BeginDragDropSource()) {
                const char* payload = name.c_str();
                ImGui::SetDragDropPayload("MATERIAL_NAME", payload, strlen(payload) + 1);

                ImGui::Image(texHandle, ImVec2(16, 16));
                ImGui::Text(payload);
                ImGui::EndDragDropSource();
            }

            ImGui::TextWrapped("%s", name.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }
    }

    ImGui::EndChild();

}
void ProjectPanel::onImGuiRender(){
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, displaySize.y * 0.7f));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y * 0.3f));

    ImGui::Begin("Project Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    checkWindowFocus(m_Engine->getDispatcher());

    drawSearchBar();

    drawFolderTree();

    ImGui::SameLine();

    drawAssetGrid();

    //drawMeshList(m_Engine->getModelLibrary().getAllMeshes());

    ImGui::End();
}
