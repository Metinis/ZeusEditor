#include "Application.h"

#include <glm/gtc/type_ptr.inl>

#include "../imgui/ImGUILayer.h"
#include "ZeusEngineCore/IShader.h"
#include "ZeusEngineCore/MaterialManager.h"
#include "ZeusEngineCore/MeshManager.h"
#include "src/config.h"
#include <iostream>

Application::Application(RendererAPI api) : m_API(api) {
    Init();
}
Application::~Application() {
    Shutdown(); 
}
void Application::Init() {
    const bool useVulkan = (m_API == RendererAPI::Vulkan);
    m_Window = std::make_unique<Window>(1280, 720, "Zeus Editor", useVulkan);
    m_MaterialManager = std::make_unique<MaterialManager>();

    
    WindowHandle handle{};
    handle.nativeWindowHandle = static_cast<void*>(m_Window->GetNativeWindow());

    RendererInitInfo initInfo{};
    initInfo.windowHandle = handle;

    m_Renderer = IRenderer::Create(m_API);
    m_Renderer->Init(initInfo);
    m_Running = true;

    ShaderInfo shaderInfo{};
    shaderInfo.api = m_API;
    shaderInfo.backendData = m_Renderer->GetShaderInfo();
    m_ShaderManager = std::make_unique<ShaderManager>(shaderInfo);

    m_ImGuiLayer = ImGUILayer::Create(m_API);
    ImGuiCreateInfo imguiCreateInfo{};
    imguiCreateInfo.window = m_Window->GetNativeWindow();
    imguiCreateInfo.api = m_API;
    imguiCreateInfo.backendData = m_Renderer->GetContext();
    m_ImGuiLayer->Init(imguiCreateInfo);



    std::string resourceRoot = RESOURCE_ROOT;
    std::string vertPath = resourceRoot + "/shaders/vkbasic.vert.spv";
    std::string fragPath = resourceRoot + "/shaders/vkbasic.frag.spv";

    std::cout << "[Shader Load] vert: " << vertPath << "\n";
    std::cout << "[Shader Load] frag: " << fragPath << "\n";
    auto shader = m_ShaderManager->Load("Basic", resourceRoot + "/shaders/vkbasic.vert.spv", resourceRoot + "/shaders/vkbasic.frag.spv");
    m_Material = m_MaterialManager->Load("Basic", shader);

    //m_ImGuiLayer->Init(m_Window->GetNativeWindow());
    /*
    m_Running = true;

    //todo move this into a scene class
    std::string resourceRoot = RESOURCE_ROOT;
    auto shader = ShaderManager::Load("Basic", resourceRoot + "/shaders/basic.vert", resourceRoot + "/shaders/basic.frag", m_API);

    m_Material = MaterialManager::Load("Basic", shader);


    std::vector<Vertex> vertices = {
        // Triangle
        Vertex({-0.5f, -0.5f, 0.0f}, {0,0,1}, {0,0}, {1,0,0,1}),
        Vertex({ 0.5f, -0.5f, 0.0f}, {0,0,1}, {1,0}, {0,1,0,1}),
        Vertex({ 0.0f,  0.5f, 0.0f}, {0,0,1}, {0.5f,1}, {0,0,1,1}),
    };

    std::vector<uint32_t> indices = {0, 1, 2};

    m_Mesh = MeshManager::Load("Triangle", vertices, indices, m_API);*/
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
    //check if valid frame
    if (!m_Renderer->BeginFrame()) return;

    m_ImGuiLayer->BeginFrame();

    ImGui::ShowDemoWindow();
    m_ImGuiLayer->Render();

    auto transform = glm::mat4(1.0);
    m_Renderer->Submit(transform, m_Material, m_Mesh);

    //inject IMGUI render
    m_Renderer->EndFrame(m_ImGuiLayer->callback);

}






