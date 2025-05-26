#pragma once
#include <memory>
#include <glm/vec4.hpp>

#include <ZeusEngineCore/Window.h>
#include "../imgui/ImGUILayer.h"
#include <ZeusEngineCore/IRenderer.h>
#include <ZeusEngineCore/RendererAPI.h>

class Application {
public:
    Application(RendererAPI api);
    ~Application();
    void Run(); //Entry point (main.cpp) calls this
private:
    void Init();
    void Shutdown();

    void ProcessEvents();
    void Update(float deltaTime);
    void Render();

    RendererAPI m_API;
    std::unique_ptr<IRenderer> m_Renderer;
    std::unique_ptr<ImGUILayer> m_ImGuiLayer;
    std::unique_ptr<Window> m_Window;
    glm::vec4 m_TriangleColor = {1.0f, 0.5f, 0.2f, 1.0f};
    bool m_Running = false;
};
