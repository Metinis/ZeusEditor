#pragma once
#include <memory>
#include <glm/vec4.hpp>
#include <vector>
#include "../imgui/ImGUILayer.h"
#include <ZeusEngineCore/API.h>
#include <ZeusEngineCore/Window.h>
#include <ZeusEngineCore/Scene.h>

#include "Editor.h"


namespace ZEN {
    class ZEngine;
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
        std::unique_ptr<ZEN::ZEngine> m_Engine{};
        std::unique_ptr<ImGUILayer> m_ImGuiLayer{};
        std::unique_ptr<ZEN::Window> m_Window{};

        //UI
        std::unique_ptr<Editor> m_Editor{};

        bool m_Running { false };

        ZEN::eRendererAPI m_API{};
    };
}
