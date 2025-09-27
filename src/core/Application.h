#pragma once
#include <memory>
#include <glm/vec4.hpp>
#include <vector>
#include "../imgui/ImGUILayer.h"
#include <ZeusEngineCore/API.h>
#include <ZeusEngineCore/Window.h>
#include <ZeusEngineCore/Scene.h>

#include "../imgui/panels/InspectorPanel.h"
#include "../imgui/panels/ScenePanel.h"
#include "../imgui/panels/ProjectPanel.h"
#include "../imgui/panels/ViewPanel.h"

namespace ZEN {
    class Scene;
}

namespace ZEN {
    class Renderer;
    class RenderSystem;
    class CameraSystem;
    class ModelImporter;
    class ModelLibrary;
}

namespace ZED {
    class Application {
    public:
        explicit Application(ZEN::eRendererAPI api);

        ~Application();

        void run(); //Entry point (main.cpp) calls this
    private:

        void processEvents();

        void onUpdate(float deltaTime);

        void onRender();

        void onUIRender();

        //Core
        std::unique_ptr<ImGUILayer> m_ImGuiLayer{};
        std::unique_ptr<ZEN::Window> m_Window{};
        std::unique_ptr<ZEN::Scene> m_Scene{};
        std::unique_ptr<ZEN::Renderer> m_Renderer{};

        //Libraries/Loaders
        std::unique_ptr<ZEN::ModelImporter> m_ModelImporter{};
        std::unique_ptr<ZEN::ModelLibrary> m_ModelLibrary{};

        //Systems
        std::unique_ptr<ZEN::RenderSystem> m_RenderSystem{};
        std::unique_ptr<ZEN::CameraSystem> m_CameraSystem{};

        //UI
        std::unique_ptr<ViewPanel> m_ViewPanel{};
        std::unique_ptr<InspectorPanel> m_InspectorPanel{};
        std::unique_ptr<ProjectPanel> m_ProjectPanel{};
        std::unique_ptr<ScenePanel> m_ScenePanel{};

        bool m_Running { false };

        ZEN::eRendererAPI m_API{};
    };
}
