#include "Application.h"
#include "src/config.h"
#include "ZeusEngineCore/Vertex.h"
#include <iostream>
#include <memory>
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/RenderSystem.h>
#include <ZeusEngineCore/Scene.h>

using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {
    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", m_API);
    m_Scene = std::make_unique<ZEN::Scene>();
    m_Renderer = std::make_unique<ZEN::Renderer>(m_Window->getNativeWindow());

    std::string resourceRoot = RESOURCE_ROOT;
    uint32_t defaultShader = m_Renderer->getContext().getResourceManager().createShader(
        resourceRoot + "/shaders/glbasic4.1.vert", resourceRoot + "/shaders/glbasic4.1.frag");

    m_RenderSystem = std::make_unique<ZEN::RenderSystem>(m_Renderer.get(),
        ZEN::ShaderComp{.shaderID = defaultShader});

    m_ImGuiLayer = ImGUILayer::create(m_Window->getNativeWindow(), api);

    m_Running = true;

    ZEN::MeshComp mesh;

    mesh.vertices = {
        // Top-left
        { { -0.5f,  0.5f, 0.0f },   // Position
          {  0.0f,  0.0f, 1.0f },   // Normal
          {  0.0f,  1.0f },         // TexCoords
          {  1.0f,  1.0f, 1.0f, 1.0f } }, // Color

        // Top-right
        { {  0.5f,  0.5f, 0.0f },
          {  0.0f,  0.0f, 1.0f },
          {  1.0f,  1.0f },
          {  1.0f,  1.0f, 1.0f, 1.0f } },

        // Bottom-right
        { {  0.5f, -0.5f, 0.0f },
          {  0.0f,  0.0f, 1.0f },
          {  1.0f,  0.0f },
          {  1.0f,  1.0f, 1.0f, 1.0f } },

        // Bottom-left
        { { -0.5f, -0.5f, 0.0f },
          {  0.0f,  0.0f, 1.0f },
          {  0.0f,  0.0f },
          {  1.0f,  1.0f, 1.0f, 1.0f } }
    };

    mesh.indices = {
        0, 1, 2,   // First triangle
        2, 3, 0    // Second triangle
    };

    entt::entity entity = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity, mesh);
}
Application::~Application() {
     
}

void Application::run() {
    while(m_Running && !m_Window->shouldClose()) {
        m_Window->pollEvents();

        const float dt = m_Window->getDeltaTime();
        onUpdate(dt);
        onRender();
    }
}
void Application::processEvents() {
    
}
void Application::onUpdate(float deltaTime) {
    m_RenderSystem->onUpdate(m_Scene->getRegistry());
}
void Application::onUIRender() {
    m_ImGuiLayer->beginFrame();
    ImGui::ShowDemoWindow();
    m_ImGuiLayer->render();
    m_ImGuiLayer->endFrame(nullptr);
}

void Application::onRender() {
    m_Renderer->beginFrame();
    m_RenderSystem->onRender(m_Scene->getRegistry());
    onUIRender();
    m_Renderer->endFrame();
}






