#include "Application.h"
#include "src/config.h"
#include "ZeusEngineCore/Vertex.h"
#include <iostream>
#include <memory>
#include <string>
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/RenderSystem.h>
#include <ZeusEngineCore/Scene.h>
#include <ZeusEngineCore/CameraSystem.h>

using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {
    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", m_API);
    m_Scene = std::make_unique<ZEN::Scene>();
    m_Renderer = std::make_unique<ZEN::Renderer>(m_API, m_Window->getNativeWindow());

    std::string resourceRoot = RESOURCE_ROOT;
    uint32_t defaultShaderID = m_Renderer->getContext()->getResourceManager().createShader(
        resourceRoot + "/shaders/glbasic4.1.vert", resourceRoot + "/shaders/glbasic4.1.frag");
    ZEN::MaterialComp defaultShader {.shaderID = defaultShaderID};
    m_Renderer->setDefaultShader(defaultShader);

    m_RenderSystem = std::make_unique<ZEN::RenderSystem>(m_Renderer.get(), m_Scene.get());

    m_ImGuiLayer = ImGUILayer::create(m_Window->getNativeWindow(), api);

    m_Running = true;

    ZEN::MeshComp mesh;

    mesh.vertices = {
        // Front face (z = +0.5)
        {{-0.5f,  0.5f,  0.5f}, {0,0,1}, {0.0f, 1.0f}}, // Top-left
        {{ 0.5f,  0.5f,  0.5f}, {0,0,1}, {1.0f, 1.0f}}, // Top-right
        {{ 0.5f, -0.5f,  0.5f}, {0,0,1}, {1.0f, 0.0f}}, // Bottom-right
        {{-0.5f, -0.5f,  0.5f}, {0,0,1}, {0.0f, 0.0f}}, // Bottom-left

        // Back face (z = -0.5)
        {{ 0.5f,  0.5f, -0.5f}, {0,0,-1}, {0.0f, 1.0f}}, // Top-left
        {{-0.5f,  0.5f, -0.5f}, {0,0,-1}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f, -0.5f, -0.5f}, {0,0,-1}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f, -0.5f, -0.5f}, {0,0,-1}, {0.0f, 0.0f}}, // Bottom-left

        // Left face (x = -0.5)
        {{-0.5f,  0.5f, -0.5f}, {-1,0,0}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1,0,0}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1,0,0}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1,0,0}, {0.0f, 0.0f}},

        // Right face (x = +0.5)
        {{ 0.5f,  0.5f,  0.5f}, {1,0,0}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1,0,0}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1,0,0}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1,0,0}, {0.0f, 0.0f}},

        // Top face (y = +0.5)
        {{-0.5f,  0.5f, -0.5f}, {0,1,0}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0,1,0}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0,1,0}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0,1,0}, {0.0f, 0.0f}},

        // Bottom face (y = -0.5)
        {{-0.5f, -0.5f,  0.5f}, {0,-1,0}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0,-1,0}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0,-1,0}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0,-1,0}, {0.0f, 0.0f}}
    };

    mesh.indices = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        4, 5, 6, 6, 7, 4,
        // Left
        8, 9,10,10,11, 8,
        // Right
       12,13,14,14,15,12,
        // Top
       16,17,18,18,19,16,
        // Bottom
       20,21,22,22,23,20
    };

    uint32_t textureID = m_Renderer->getContext()->getResourceManager().createTexture(
        resourceRoot + "/textures/container2.png");
    uint32_t specularID = m_Renderer->getContext()->getResourceManager().createTexture(
        resourceRoot + "/textures/container2_specular.png");
    ZEN::MaterialComp comp {
        .shaderID = defaultShaderID,
        .textureID = textureID,
        .specularTexID = specularID,
        .specular = 0.5f,
        .shininess = 32,
    };

    entt::entity entity = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity, mesh);
    m_Scene->getRegistry().emplace<ZEN::TransformComp>(entity,
        ZEN::TransformComp{.position = {0.0f, 0.0f, -3.0f}});
    m_Scene->getRegistry().emplace<ZEN::MaterialComp>(entity, comp);

    entt::entity entity2 = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity2, mesh);
    m_Scene->getRegistry().emplace<ZEN::TransformComp>(entity2,
        ZEN::TransformComp{.position = {2.0f, 0.0f, -3.0f}});
    m_Scene->getRegistry().emplace<ZEN::MaterialComp>(entity2, comp);

    entt::entity cameraEntity = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::CameraComp>(cameraEntity);
    m_Scene->getRegistry().emplace<ZEN::TransformComp>(cameraEntity);

    entt::entity skyboxEntity = m_Scene->createEntity();
    ZEN::MeshComp skyboxMesh{};
    skyboxMesh.vertices = {
        {{-1.0f,  1.0f, -1.0f}}, {{-1.0f, -1.0f, -1.0f}}, {{ 1.0f, -1.0f, -1.0f}}, {{ 1.0f,  1.0f, -1.0f}}, // Back
        {{-1.0f, -1.0f,  1.0f}}, {{-1.0f,  1.0f,  1.0f}}, {{ 1.0f,  1.0f,  1.0f}}, {{ 1.0f, -1.0f,  1.0f}}, // Front
        {{-1.0f,  1.0f,  1.0f}}, {{-1.0f, -1.0f,  1.0f}}, {{-1.0f, -1.0f, -1.0f}}, {{-1.0f,  1.0f, -1.0f}}, // Left
        {{ 1.0f,  1.0f, -1.0f}}, {{ 1.0f, -1.0f, -1.0f}}, {{ 1.0f, -1.0f,  1.0f}}, {{ 1.0f,  1.0f,  1.0f}}, // Right
        {{-1.0f,  1.0f,  1.0f}}, {{-1.0f,  1.0f, -1.0f}}, {{ 1.0f,  1.0f, -1.0f}}, {{ 1.0f,  1.0f,  1.0f}}, // Top
        {{-1.0f, -1.0f, -1.0f}}, {{-1.0f, -1.0f,  1.0f}}, {{ 1.0f, -1.0f,  1.0f}}, {{ 1.0f, -1.0f, -1.0f}}  // Bottom
    };

    // Cube indices
    skyboxMesh.indices = {
        0,1,2, 2,3,0,       // Back
        4,5,6, 6,7,4,       // Front
        8,9,10, 10,11,8,    // Left
        12,13,14, 14,15,12, // Right
        16,17,18, 18,19,16, // Top
        20,21,22, 22,23,20  // Bottom
    };

    ZEN::SkyboxComp skyboxComp{};
    skyboxComp.shaderID = m_Renderer->getContext()->getResourceManager().createShader(
        resourceRoot + "/shaders/glskybox.vert", resourceRoot + "/shaders/glskybox.frag");
    skyboxComp.textureID = m_Renderer->getContext()->getResourceManager().createCubeMapTexture(resourceRoot + "/textures/skybox/");
    m_Scene->getRegistry().emplace<ZEN::SkyboxComp>(skyboxEntity, skyboxComp);
    m_Scene->getRegistry().emplace<ZEN::MeshComp>(skyboxEntity, skyboxMesh);


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

static auto const inspectTransform = [](ZEN::TransformComp& out) {
    ImGui::DragFloat3("position", &out.position.x, 0.01f);
    ImGui::DragFloat3("rotation", &out.rotation.x);
    ImGui::DragFloat3("scale", &out.scale.x, 0.01f, 0.0f, 100.0f);
};
void Application::onUIRender() {
    m_ImGuiLayer->beginFrame();

    // Show demo window (optional)
    ImGui::ShowDemoWindow();

    // Start a new window for camera controls
    ImGui::Begin("Camera Controls");

    auto cameraView = m_Scene->getRegistry().view<ZEN::CameraComp, ZEN::TransformComp>();
    for (auto entity : cameraView) {
        auto &camera = m_Scene->getRegistry().get<ZEN::CameraComp>(entity);
        auto &transform = m_Scene->getRegistry().get<ZEN::TransformComp>(entity);
        if (camera.isPrimary) {
           const char* label = "Primary Camera";
            if (ImGui::TreeNode(label)) {
                inspectTransform(transform);
                ImGui::TreePop();
            }
        }
    }

    ImGui::End();

    ImGui::Begin("Transform Controls");

    auto transformView = m_Scene->getRegistry().view<ZEN::TransformComp, ZEN::MeshComp>();
    for (auto entity : transformView) {
        auto &transform = m_Scene->getRegistry().get<ZEN::TransformComp>(entity);
        auto const label = "Entity: " + std::to_string((int)entity);
        if (ImGui::TreeNode(label.c_str())) {
            inspectTransform(transform);
            ImGui::TreePop();
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






