#include "MenuBarPanel.h"

MenuBarPanel::MenuBarPanel(ZEN::ZEngine *engine, SelectionContext& selection) : m_Engine(engine), m_SelectionContext(selection)  {
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
                m_Engine->getRenderSystem().toggleDrawNormals();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Run")) {
            if (ImGui::MenuItem("Run Project")) {
                ZEN::RunPlayModeEvent e(true);
                ZEN::Application::get().callEvent(e);

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void MenuBarPanel::onEvent(ZEN::Event &event) {
    Layer::onEvent(event);
}

bool MenuBarPanel::onPlayModeEvent(ZEN::RunPlayModeEvent &e) {
    return false;
}
