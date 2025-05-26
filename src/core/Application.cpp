#include "Application.h"
#include "../imgui/ImGUILayer.h"

Application::Application(RendererAPI api) : m_API(api) {
    Init();
}
Application::~Application() {
    Shutdown();
}
void Application::Init() {
    const bool useVulkan = (m_API == RendererAPI::Vulkan);
    m_Window = std::make_unique<Window>(1280, 720, "Zeus Engine", useVulkan);

    m_Renderer = IRenderer::Create(m_API);
    m_Renderer->Init();

    m_ImGuiLayer = ImGUILayer::Create(m_API);
    m_ImGuiLayer->Init(m_Window->GetNativeWindow());

    m_Running = true;
}
void Application::Shutdown() {
    if(m_ImGuiLayer) {
        m_ImGuiLayer->Shutdown();
    }
    if(m_Renderer) {
        m_Renderer->Cleanup();
    }
}
void Application::Run() {
    while(m_Running && !m_Window->ShouldClose()) {
        m_Window->PollEvents();

        float dt = m_Window->GetDeltaTime();
        Update(dt);
        Render();

        if(m_API == RendererAPI::OpenGL) {
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
    m_Renderer->BeginFrame();

    m_Renderer->DrawMesh(m_TriangleColor);
    m_ImGuiLayer->BeginFrame();
    ImGui::ColorEdit4("Triangle Color", &m_TriangleColor[0]);
    m_ImGuiLayer->Render();

    m_Renderer->EndFrame();
}






