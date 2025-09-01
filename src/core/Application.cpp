#include "Application.h"
#include "src/config.h"
#include "ZeusEngineCore/Vertex.h"
#include <iostream>
#include <memory>
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/RenderSystem.h>
#include <ZeusEngineCore/Scene.h>

#include "../../../ZeusEngineCore/src/Systems/Scene/CameraSystem.h"

using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {
    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", m_API);
    m_Scene = std::make_unique<ZEN::Scene>();
    m_Renderer = std::make_unique<ZEN::Renderer>(m_API, m_Window->getNativeWindow());

    std::string resourceRoot = RESOURCE_ROOT;
    uint32_t defaultShader = m_Renderer->getContext()->getResourceManager().createShader(
        resourceRoot + "/shaders/glbasic4.1.vert", resourceRoot + "/shaders/glbasic4.1.frag");

    m_RenderSystem = std::make_unique<ZEN::RenderSystem>(m_Renderer.get(),
        ZEN::ShaderComp{.shaderID = defaultShader});

    m_ImGuiLayer = ImGUILayer::create(m_Window->getNativeWindow(), api);

    m_Running = true;

    ZEN::MeshComp mesh;

    mesh.vertices = {
        // Front face (z = 0.5)
        {{-0.5f,  0.5f,  0.5f}, {0,0,1}, {0,1}, {1,0,0,1}}, // 0 top-left-front
        {{ 0.5f,  0.5f,  0.5f}, {0,0,1}, {1,1}, {0,1,0,1}}, // 1 top-right-front
        {{ 0.5f, -0.5f,  0.5f}, {0,0,1}, {1,0}, {0,0,1,1}}, // 2 bottom-right-front
        {{-0.5f, -0.5f,  0.5f}, {0,0,1}, {0,0}, {1,1,0,1}}, // 3 bottom-left-front

        // Back face (z = -0.5)
        {{-0.5f,  0.5f, -0.5f}, {0,0,-1}, {1,1}, {1,0,1,1}}, // 4 top-left-back
        {{ 0.5f,  0.5f, -0.5f}, {0,0,-1}, {0,1}, {0,1,1,1}}, // 5 top-right-back
        {{ 0.5f, -0.5f, -0.5f}, {0,0,-1}, {0,0}, {1,1,1,1}}, // 6 bottom-right-back
        {{-0.5f, -0.5f, -0.5f}, {0,0,-1}, {1,0}, {0,0,0,1}}  // 7 bottom-left-back
    };

    mesh.indices = {
        // Front
        0,1,2,  2,3,0,
        // Right
        1,5,6,  6,2,1,
        // Back
        5,4,7,  7,6,5,
        // Left
        4,0,3,  3,7,4,
        // Top
        4,5,1,  1,0,4,
        // Bottom
        3,2,6,  6,7,3
    };

    entt::entity entity = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity, mesh);

    entt::entity cameraEntity = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::CameraComp>(cameraEntity);
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
    ZEN::CameraSystem::onUpdate(m_Scene->getRegistry(), m_Window->getWidth(),
        m_Window->getHeight());
    m_RenderSystem->onUpdate(m_Scene->getRegistry());
}
void Application::onUIRender() {
    m_ImGuiLayer->beginFrame();

    // Show demo window (optional)
    ImGui::ShowDemoWindow();

    // Start a new window for camera controls
    ImGui::Begin("Camera Controls");

    auto view = m_Scene->getRegistry().view<ZEN::CameraComp>();
    for (auto entity : view) {
        auto &camera = m_Scene->getRegistry().get<ZEN::CameraComp>(entity);
        if (camera.isPrimary) {
            // Create sliders for position
            ImGui::SliderFloat("Position X", &camera.position.x, -10.0f, 10.0f);
            ImGui::SliderFloat("Position Y", &camera.position.y, -10.0f, 10.0f);
            ImGui::SliderFloat("Position Z", &camera.position.z, -10.0f, 10.0f);

            // Optionally sliders for front direction
            ImGui::SliderFloat("Front X", &camera.front.x, -1.0f, 1.0f);
            ImGui::SliderFloat("Front Y", &camera.front.y, -1.0f, 1.0f);
            ImGui::SliderFloat("Front Z", &camera.front.z, -1.0f, 1.0f);

            // Normalize front to avoid stretching
            camera.front = glm::normalize(camera.front);
            break; // Only control the first primary camera
        }
    }

    ImGui::End();

    m_ImGuiLayer->render();
    m_ImGuiLayer->endFrame(nullptr);
}

void Application::onRender() {
    m_Renderer->beginFrame();
    m_RenderSystem->onRender(m_Scene->getRegistry());
    onUIRender();
    m_Renderer->endFrame();
}






