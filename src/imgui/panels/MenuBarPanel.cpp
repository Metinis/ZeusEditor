
#include "MenuBarPanel.h"
#include <ZeusEngineCore/ZEngine.h>
#include <ZeusEngineCore/EventDispatcher.h>
#include <ZeusEngineCore/InputEvents.h>

#include <imgui.h>

MenuBarPanel::MenuBarPanel(ZEN::ZEngine *engine) : m_Engine(engine) {
}

void MenuBarPanel::onImGuiRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {}
            if (ImGui::MenuItem("Save")) {}
            if (ImGui::MenuItem("Exit")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {}
            if (ImGui::MenuItem("Redo")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Fullscreen")) {}
            if (ImGui::MenuItem("Toggle Normals")) {
                m_Engine->getDispatcher().trigger<ZEN::ToggleDrawNormalsEvent>(ZEN::ToggleDrawNormalsEvent{});
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
