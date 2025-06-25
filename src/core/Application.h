#pragma once
#include <memory>
#include <glm/vec4.hpp>
#include "ZeusEngineCore/ShaderManager.h"
#include "ZeusEngineCore/MaterialManager.h"
#include <ZeusEngineCore/Window.h>
#include "../imgui/ImGUILayer.h"
#include <ZeusEngineCore/IRenderer.h>
#include <ZeusEngineCore/RendererAPI.h>

#include "ZeusEngineCore/Material.h"

class Application {
public:
    explicit Application(RendererAPI api);
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
    std::unique_ptr<ShaderManager> m_ShaderManager;
    std::unique_ptr<MaterialManager> m_MaterialManager;
    std::shared_ptr<Material> m_Material;
    std::shared_ptr<IMesh> m_Mesh;
    bool m_Running = false;
};
