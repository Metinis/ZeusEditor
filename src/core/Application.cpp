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
    std::string vertPath{};
    std::string fragPath{};
    if (m_API == ZEN::eRendererAPI::OpenGL) {
        vertPath = resourceRoot + "/shaders/glbasic4.1.vert";
        fragPath = resourceRoot + "/shaders/glbasic4.1.frag";
    }
    else {
        vertPath = resourceRoot + "/shaders/vkbasic.vert.spv";
        fragPath = resourceRoot + "/shaders/vkbasic.frag.spv";
    }
    

    std::cout << "[Shader Load] vert: " << vertPath << "\n";
    std::cout << "[Shader Load] frag: " << fragPath << "\n";
    auto shader = m_ShaderManager->Load("Basic", vertPath, fragPath);

    std::string texturePath = resourceRoot + "/textures/texture.jpg";
    auto texture = m_TextureManager->Load("Basic", texturePath);
    m_Material = m_MaterialManager->Load("Basic", shader);
    m_Material->SetTexture(texture);

    std::vector<ZEN::Vertex> vertices = {
            // Front face (z = +0.5)
            ZEN::Vertex({-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0.0f, 0.0f}, {1, 0, 0, 1}),
            ZEN::Vertex({ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1.0f, 0.0f}, {1, 0, 0, 1}),
            ZEN::Vertex({ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1.0f, 1.0f}, {1, 0, 0, 1}),
            ZEN::Vertex({-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0.0f, 1.0f}, {1, 0, 0, 1}),

            // Back face (z = -0.5)
            ZEN::Vertex({-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1.0f, 0.0f}, {0, 1, 0, 1}),
            ZEN::Vertex({ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0.0f, 0.0f}, {0, 1, 0, 1}),
            ZEN::Vertex({ 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0.0f, 1.0f}, {0, 1, 0, 1}),
            ZEN::Vertex({-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1.0f, 1.0f}, {0, 1, 0, 1}),

            // Left face (x = -0.5)
            ZEN::Vertex({-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0.0f, 0.0f}, {0, 0, 1, 1}),
            ZEN::Vertex({-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {1.0f, 0.0f}, {0, 0, 1, 1}),
            ZEN::Vertex({-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {1.0f, 1.0f}, {0, 0, 1, 1}),
            ZEN::Vertex({-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {0.0f, 1.0f}, {0, 0, 1, 1}),

            // Right face (x = +0.5)
            ZEN::Vertex({0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1.0f, 0.0f}, {1, 1, 0, 1}),
            ZEN::Vertex({0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0.0f, 0.0f}, {1, 1, 0, 1}),
            ZEN::Vertex({0.5f,  0.5f,  0.5f}, {1, 0, 0}, {0.0f, 1.0f}, {1, 1, 0, 1}),
            ZEN::Vertex({0.5f,  0.5f, -0.5f}, {1, 0, 0}, {1.0f, 1.0f}, {1, 1, 0, 1}),

            // Top face (y = +0.5)
            ZEN::Vertex({-0.5f, 0.5f,  0.5f}, {0, 1, 0}, {0.0f, 0.0f}, {1, 0, 1, 1}),
            ZEN::Vertex({ 0.5f, 0.5f,  0.5f}, {0, 1, 0}, {1.0f, 0.0f}, {1, 0, 1, 1}),
            ZEN::Vertex({ 0.5f, 0.5f, -0.5f}, {0, 1, 0}, {1.0f, 1.0f}, {1, 0, 1, 1}),
            ZEN::Vertex({-0.5f, 0.5f, -0.5f}, {0, 1, 0}, {0.0f, 1.0f}, {1, 0, 1, 1}),

            // Bottom face (y = -0.5)
            ZEN::Vertex({-0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1.0f, 1.0f}, {0, 1, 1, 1}),
            ZEN::Vertex({ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {0.0f, 1.0f}, {0, 1, 1, 1}),
            ZEN::Vertex({ 0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0.0f, 0.0f}, {0, 1, 1, 1}),
            ZEN::Vertex({-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1.0f, 0.0f}, {0, 1, 1, 1}),
    };


    std::vector<uint32_t> indices = {
            // Front
            0, 1, 2,  2, 3, 0,

            // Back
            4, 5, 6,  6, 7, 4,

            // Left
            8, 9,10, 10,11, 8,

            // Right
            12,13,14, 14,15,12,

            // Top
            16,17,18, 18,19,16,

            // Bottom
            20,21,22, 22,23,20,
    };



    m_Mesh = m_MeshManager->Load("Triangle", vertices, indices);
    m_Commands.push_back(ZEN::RenderCommand{
        .mesh = m_Mesh,
        .material = m_Material
    });
    ZEN::Transform transform{};
    transform.position = {0.0f, 0.0f, -5.0f};
    transform.rotation = {0.0f, 50.0f, 0.0f};
    m_Commands.at(0).transforms.push_back(transform);

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
    }
}
void Application::ProcessEvents() {
    //handle input/resize, etc
}
void Application::Update(float deltaTime) {
    //Update Scene here
}
static auto const inspectTransform = [](ZEN::Transform& out) {
    ImGui::DragFloat3("position", &out.position.x, 0.01f);
    ImGui::DragFloat3("rotation", &out.rotation.x);
    ImGui::DragFloat3("scale", &out.scale.x, 0.01f, 0.0f, 100.0f);
};
void Application::Render() {
    //check if valid frame
    if (!m_Renderer->BeginFrame()) return;
    m_Renderer->UpdateInstances(m_Commands.at(0).transforms);

    m_ImGuiLayer->BeginFrame();

    ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize({300.0f, 700.0f}, ImGuiCond_Once);
    auto& shader = m_Material->GetShader();
    if (ImGui::Begin("Inspect")) {
        if (ImGui::Checkbox("wireframe", shader->GetWireframeFlag())) {
            //shader->ToggleWireframe();
        }
#ifndef __APPLE__
        if (*shader->GetWireframeFlag() == 1) {
            auto const& lineWidthRange = std::array<float, 2>{0.0f, 100.0f};
            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("line width", shader->GetLineWidth(), 0.25f,
                             lineWidthRange[0], lineWidthRange[1]);
        }
#endif
    }
    ImGui::Separator();
    if (ImGui::TreeNode("View")) {
        inspectTransform(m_Renderer->ViewMatrix());
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Instances")) {
        if (ImGui::Button("Add Instance")) {
            m_Commands.at(0).transforms.push_back(ZEN::Transform{});
        }
        for (std::size_t i = 0; i < m_Commands.at(0).transforms.size(); ++i) {
            auto const label = "Instance: " + std::to_string(i);
            if (ImGui::TreeNode(label.c_str())) {
                inspectTransform(m_Commands.at(0).transforms.at(i));
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
    m_ImGuiLayer->Render();

    auto transform = glm::mat4(1.0);
    m_Renderer->Submit(m_Commands.at(0).transforms, m_Commands.at(0).mesh, m_Commands.at(0).material);

    //inject IMGUI render

    m_Renderer->EndFrame(m_ImGuiLayer->callback);
}






