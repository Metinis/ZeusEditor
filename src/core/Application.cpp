#include "Application.h"
#include "ZeusEngineCore/ShaderManager.h"
#include "ZeusEngineCore/MaterialManager.h"
#include <ZeusEngineCore/Window.h>
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/IRendererAPI.h>
#include "ZeusEngineCore/Material.h"
#include "ZeusEngineCore/MeshManager.h"
#include "ZeusEngineCore/TextureManager.h"
#include "src/config.h"
#include "../imgui/ImGUILayer.h"
#include "ZeusEngineCore/Utils.h"
#include "ZeusEngineCore/Vertex.h"
#include "ZeusEngineCore/IShader.h"
#include <iostream>

using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {
    Init();
}
Application::~Application() {
    Shutdown(); 
}
void Application::Init() {
    const bool useVulkan = (m_API == ZEN::eRendererAPI::Vulkan);
    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", useVulkan);
    m_MaterialManager = std::make_unique<ZEN::MaterialManager>();

    ZEN::WindowHandle handle{};
    handle.nativeWindowHandle = m_Window->GetNativeWindow();

    ZEN::RendererInitInfo initInfo{};
    initInfo.windowHandle = handle;
    initInfo.api = m_API;

    m_Renderer = std::make_unique<ZEN::Renderer>(initInfo);//ZEN::IRenderer::Create(m_API);
    m_Running = true;

    //todo use std::moves
    m_ShaderManager = std::make_unique<ZEN::ShaderManager>(m_Renderer->GetAPIBackend(),
                                                           m_Renderer->GetAPIRenderer());

    m_ImGuiLayer = ImGUILayer::Create(m_Window->GetNativeWindow(),
                                      m_Renderer->GetAPIBackend());
    m_MeshManager = std::make_unique<ZEN::MeshManager>(m_Renderer->GetAPIBackend(),
                                                       m_Renderer->GetAPIRenderer());

    m_TextureManager = std::make_unique<ZEN::TextureManager>(m_Renderer->GetAPIBackend(),
                                                             m_Renderer->GetAPIRenderer());

    std::string resourceRoot = RESOURCE_ROOT;
    std::string vertPath = resourceRoot + "/shaders/vkbasic.vert.spv";
    std::string fragPath = resourceRoot + "/shaders/vkbasic.frag.spv";

    std::cout << "[Shader Load] vert: " << vertPath << "\n";
    std::cout << "[Shader Load] frag: " << fragPath << "\n";
    auto shader = m_ShaderManager->Load("Basic", vertPath, fragPath);
    auto texture = m_TextureManager->Load("Basic", "");
    m_Material = m_MaterialManager->Load("Basic", shader);
    m_Material->SetTexture(texture);

    std::vector<ZEN::Vertex> vertices = {
            ZEN::Vertex({-200.0f, -200.0f, 0.0f}, {0,0,1}, {0.0f, 0.0f}, {1,0,0,1}),  // 0
            ZEN::Vertex({ 200.0f, -200.0f, 0.0f}, {0,0,1}, {1.0f, 0.0f}, {0,1,0,1}),  // 1
            ZEN::Vertex({ 200.0f,  200.0f, 0.0f}, {0,0,1}, {1.0f, 1.0f}, {0,0,1,1}),  // 2
            ZEN::Vertex({-200.0f,  200.0f, 0.0f}, {0,0,1}, {0.0f, 1.0f}, {1,1,0,1}),  // 3
    };

    std::vector<uint32_t> indices = {
            0, 1, 2,  // First triangle
            2, 3, 0   // Second triangle
    };


    m_Mesh = m_MeshManager->Load("Triangle", vertices, indices);
}
void Application::Shutdown() {
    //smart pointers clear automatically
}
void Application::Run() {
    while(m_Running && !m_Window->ShouldClose()) {
        m_Window->PollEvents();

        const float dt = m_Window->GetDeltaTime();
        Update(dt);
        Render();

        if(m_API == ZEN::eRendererAPI::OpenGL) {
            m_Window->SwapBuffers();
        }
    }
}
void Application::ProcessEvents() {
    //handle input/resize, etc
}
void Application::Update(float deltaTime) {
    //Update Scene here
}
void Application::Render() {
    //check if valid frame
    if (!m_Renderer->BeginFrame()) return;

    m_ImGuiLayer->BeginFrame();

    ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize({200.0f, 100.0f}, ImGuiCond_Once);
    auto& shader = m_Material->GetShader();
    if (ImGui::Begin("Inspect")) {
        if (ImGui::Checkbox("wireframe", shader->GetWireframeFlag())) {
            //shader->ToggleWireframe();
        }
#ifndef __APPLE__
        if (*shader->GetWireframeFlag() == 1) {
            auto const& line_width_range = std::array<float, 2>{0.0f, 100.0f};
            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("line width", shader->GetLineWidth(), 0.25f,
                             line_width_range[0], line_width_range[1]);
        }
#endif
    }
    ImGui::Separator();
    if (ImGui::TreeNode("View")) {
        ImGui::DragFloat2("position", &m_Renderer->ViewMatrix().position.x);
        ImGui::DragFloat("rotation", &m_Renderer->ViewMatrix().rotation);
        ImGui::DragFloat2("scale", &m_Renderer->ViewMatrix().scale.x);
        ImGui::TreePop();
    }
    ImGui::End();
    m_ImGuiLayer->Render();

    auto transform = glm::mat4(1.0);
    m_Renderer->Submit(transform, m_Material, m_Mesh);

    //inject IMGUI render
    m_Renderer->EndFrame(m_ImGuiLayer->callback);

}






