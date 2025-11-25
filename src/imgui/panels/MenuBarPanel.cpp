#include "MenuBarPanel.h"

MenuBarPanel::MenuBarPanel(ZEN::ZEngine *engine, SelectionContext& selection) : m_Engine(engine), m_SelectionContext(selection)  {
    //m_Engine->getDispatcher().attach<ZEN::ToggleEditorEvent, MenuBarPanel, &MenuBarPanel::onToggleEditor>(this);
}

void MenuBarPanel::onUIRender() {
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
                //m_Engine->getDispatcher().trigger<ZEN::ToggleDrawNormalsEvent>(ZEN::ToggleDrawNormalsEvent{});
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Run")) {
            if (ImGui::MenuItem("Run Project")) {
                //m_Engine->getDispatcher().trigger<ZEN::ToggleEditorEvent>(ZEN::ToggleEditorEvent{});
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
/*void MenuBarPanel::onToggleEditor(ZEN::ToggleEditorEvent &e) {
    //ZEN::Application::get().popOverlay(this);
}*/
