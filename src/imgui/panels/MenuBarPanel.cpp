#include "MenuBarPanel.h"

MenuBarPanel::MenuBarPanel(ZEN::ZEngine *engine, SelectionContext& selection) : m_Engine(engine), m_SelectionContext(selection)  {
}

void MenuBarPanel::onUIRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {
                //ZEN::AssetSerializer assetSerializer(&m_Engine->getModelLibrary());
                //assetSerializer.deserialize("/assets/default.zenpackage");
                ZEN::SceneSerializer serializer(&m_Engine->getScene());
                serializer.deserialize("/scenes/default.zen");
            }
            if (ImGui::MenuItem("Save")) {
                //ZEN::AssetSerializer assetSerializer(&m_Engine->getModelLibrary());
                //assetSerializer.serialize("/assets/default.zenpackage");
                ZEN::SceneSerializer serializer(&m_Engine->getScene());
                serializer.serialize("/scenes/default.zen");
            }
            if (ImGui::MenuItem("Exit")) {
                ZEN::Application::get().close();
            }
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
            if (ImGui::BeginMenu("Aspect Ratio")) {
                if (ImGui::MenuItem("16:9")) {
                    m_Engine->setAspectRatio(16.0f/9.0f);
                }
                if (ImGui::MenuItem("16:10")) {
                    m_Engine->setAspectRatio(16.0f/10.0f);
                }
                if (ImGui::MenuItem("4:3")) {
                    m_Engine->setAspectRatio(4.0f/3.0f);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Run")) {
            if(!m_isPLaying) {
                if (ImGui::MenuItem("Run Project")) {
                    ZEN::RunPlayModeEvent e(true);
                    ZEN::Application::get().callEvent(e);
                    m_isPLaying = true;
                }
            }
            else if(ImGui::MenuItem("Stop Project")) {
                ZEN::RunPlayModeEvent e(false);
                ZEN::Application::get().callEvent(e);
                m_isPLaying = false;
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
