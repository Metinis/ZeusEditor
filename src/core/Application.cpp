#include "Application.h"

#include <glm/gtc/type_ptr.inl>

#include "../imgui/ImGUILayer.h"
#include "ZeusEngineCore/IShader.h"
#include "ZeusEngineCore/MaterialManager.h"
#include "ZeusEngineCore/MeshManager.h"
#include "ZeusEngineCore/ShaderManager.h"

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

    //todo move this into a scene class
    std::shared_ptr<IShader> shader = ShaderManager::Load("Basic", "resources/shaders/basic.vert", "resources/shaders/basic.frag", m_API);

    m_Material = MaterialManager::Load("Basic", shader);


    std::vector<Vertex> vertices = {
        // Triangle
        Vertex({-0.5f, -0.5f, 0.0f}, {0,0,1}, {0,0}, {1,0,0,1}),
        Vertex({ 0.5f, -0.5f, 0.0f}, {0,0,1}, {1,0}, {0,1,0,1}),
        Vertex({ 0.0f,  0.5f, 0.0f}, {0,0,1}, {0.5f,1}, {0,0,1,1}),
    };

    std::vector<uint32_t> indices = {0, 1, 2};

    m_Mesh = MeshManager::Load("Triangle", vertices, indices, m_API);
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

    //m_Renderer->DrawMesh(*m_Mesh, *m_Material);
    glm::mat4 tranform;
    m_Renderer->Submit(tranform, m_Material, m_Mesh);
    m_ImGuiLayer->BeginFrame();

    ImGui::ColorEdit4("Material Color", glm::value_ptr(m_Material->ColorRef("u_Color")));

    m_Renderer->EndFrame();

    m_ImGuiLayer->Render();


}






