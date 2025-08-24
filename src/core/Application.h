#pragma once
#include <memory>
#include <glm/vec4.hpp>
#include <vector>
#include "../imgui/ImGUILayer.h"
#include <ZeusEngineCore/API.h>
#include <ZeusEngineCore/Window.h>

namespace ZED {
    class Application {
    public:
        explicit Application(ZEN::eRendererAPI api);

        ~Application();

        void run(); //Entry point (main.cpp) calls this
    private:

        void processEvents();

        void update(float deltaTime);

        void render();

        std::unique_ptr<ImGUILayer> m_ImGuiLayer{};
        std::unique_ptr<ZEN::Window> m_Window{};

        bool m_Running { false };

        ZEN::eRendererAPI m_API{};
    };
}
