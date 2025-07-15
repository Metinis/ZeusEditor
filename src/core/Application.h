#pragma once
#include <memory>
#include <glm/vec4.hpp>
#include "ZeusEngineCore/ShaderManager.h"
#include "ZeusEngineCore/MaterialManager.h"
#include <ZeusEngineCore/Window.h>
#include "../imgui/ImGUILayer.h"
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/RendererAPI.h>
#include "ZeusEngineCore/Material.h"
#include "ZeusEngineCore/MeshManager.h"

namespace ZED {
    class Application {
    public:
        explicit Application(ZEN::RendererAPI api);

        ~Application();

        void Run(); //Entry point (main.cpp) calls this
    private:
        void Init();

        void Shutdown();

        void ProcessEvents();

        void Update(float deltaTime);

        void Render();

        ZEN::RendererAPI m_API;
        std::unique_ptr<ZEN::Renderer> m_Renderer;
        std::unique_ptr<ImGUILayer> m_ImGuiLayer;
        std::unique_ptr<ZEN::Window> m_Window;
        std::unique_ptr<ZEN::ShaderManager> m_ShaderManager;
        std::unique_ptr<ZEN::MaterialManager> m_MaterialManager;
        std::unique_ptr<ZEN::MeshManager> m_MeshManager;
        std::shared_ptr<ZEN::Material> m_Material;

        std::shared_ptr<ZEN::IMesh> m_Mesh;

        bool m_Running = false;
    };
}
