#include "Application.h"
#include "src/config.h"
#include "ZeusEngineCore/Vertex.h"
#include <iostream>
#include <memory>
#include <string>
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/RenderSystem.h>
#include <ZeusEngineCore/CameraSystem.h>
#include <ZeusEngineCore/MeshLibrary.h>

using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {
    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", m_API);
    m_Scene = std::make_unique<ZEN::Scene>();
    m_Renderer = std::make_unique<ZEN::Renderer>(m_API, m_Window->getNativeWindow());

    std::string resourceRoot = RESOURCE_ROOT;
    uint32_t defaultShaderID = m_Renderer->getResourceManager()->createShader(
        resourceRoot + "/shaders/glbasic4.1.vert", resourceRoot + "/shaders/glbasic4.1.frag");
    ZEN::MaterialComp defaultShader {.shaderID = defaultShaderID};
    m_Renderer->setDefaultShader(defaultShader);

    m_RenderSystem = std::make_unique<ZEN::RenderSystem>(m_Renderer.get(), m_Scene.get());

    m_ImGuiLayer = ImGUILayer::create(m_Window->getNativeWindow(), api);

    m_Running = true;

    ZEN::MeshLibrary::init();

    uint32_t textureID = m_Renderer->getResourceManager()->createTexture(
        resourceRoot + "/textures/container2.png");
    uint32_t specularID = m_Renderer->getResourceManager()->createTexture(
        resourceRoot + "/textures/container2_specular.png");
    ZEN::MaterialComp comp {
        .shaderID = defaultShaderID,
        .textureID = textureID,
        .specularTexID = specularID,
        .specular = 0.5f,
        .shininess = 32,
    };

    entt::entity entity = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity, *ZEN::MeshLibrary::get("Cube"));
    m_Scene->getRegistry().emplace<ZEN::TransformComp>(entity,
        ZEN::TransformComp{.position = {0.0f, 0.0f, -3.0f}});
    m_Scene->getRegistry().emplace<ZEN::MaterialComp>(entity, comp);
    m_Scene->getRegistry().emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Cube 1"});

    entt::entity entity2 = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity2, *ZEN::MeshLibrary::get("Cube"));
    m_Scene->getRegistry().emplace<ZEN::TransformComp>(entity2,
        ZEN::TransformComp{.position = {2.0f, 0.0f, -3.0f}});
    m_Scene->getRegistry().emplace<ZEN::MaterialComp>(entity2, comp);
    m_Scene->getRegistry().emplace<ZEN::TagComp>(entity2, ZEN::TagComp{.tag = "Cube 2"});

    entt::entity cameraEntity = m_Scene->createEntity();
    m_Scene->getRegistry().emplace<ZEN::CameraComp>(cameraEntity);
    m_Scene->getRegistry().emplace<ZEN::TransformComp>(cameraEntity);
    m_Scene->getRegistry().emplace<ZEN::TagComp>(cameraEntity, ZEN::TagComp{.tag = "Scene Camera"});

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
    skyboxComp.shaderID = m_Renderer->getResourceManager()->createShader(
        resourceRoot + "/shaders/glskybox.vert", resourceRoot + "/shaders/glskybox.frag");
    skyboxComp.textureID = m_Renderer->getResourceManager()->createCubeMapTexture(resourceRoot + "/textures/skybox/");
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

    drawSceneViewPanel();
    drawInspectorPanel();
    drawProjectPanel();
    drawScenePanel();

    m_ImGuiLayer->render();
    m_ImGuiLayer->endFrame(nullptr);
}

void Application::drawSceneViewPanel() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.2f, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.6f, displaySize.y * 0.7f));

    ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImVec2 panelSize = ImGui::GetContentRegionAvail();
    ZEN::Texture texture = m_Renderer->getColorTexture();

    ImGui::Image(
        (void*)(intptr_t)m_Renderer->getResourceManager()->getTexture(texture.textureID),
        panelSize,
        ImVec2(0, 1), // uv0 (top-left)
        ImVec2(1, 0)  // uv1 (bottom-right)
    );

    ImGui::End();
}

void Application::drawInspectorPanel() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(displaySize.x * 0.8f, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f));
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if (m_SelectedEntity != entt::null && m_Scene->getRegistry().valid(m_SelectedEntity)) {
        if (auto* name = m_Scene->getRegistry().try_get<ZEN::TagComp>(m_SelectedEntity)) {
            char buffer[128];
            strncpy(buffer, name->tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                name->tag = buffer;
            }
        }

        if (auto* transform = m_Scene->getRegistry().try_get<ZEN::TransformComp>(m_SelectedEntity)) {
            inspectTransform(*transform);
        }

        if (auto* material = m_Scene->getRegistry().try_get<ZEN::MaterialComp>(m_SelectedEntity)) {
            ImGui::DragFloat("Specular", &material->specular, 0.01f, 0.0f, 1.0f);
            ImGui::DragInt("Shininess", &material->shininess, 1, 1, 256);
        }
    }

    ImGui::End();
}
void Application::drawProjectPanel() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, displaySize.y * 0.7f)); // bottom-left
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y * 0.3f));

    ImGui::Begin("Project Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Example: iterate over loaded assets
    /*auto& textures = m_Renderer->getResourceManager()->getAllTextures();
    if (ImGui::TreeNode("Textures")) {
        for (auto& [id, tex] : textures) {
            if (ImGui::Selectable(tex.name.c_str())) {
                // Optionally assign selected texture to the currently selected entity
            }
        }
        ImGui::TreePop();
    }*/

    auto& meshes = ZEN::MeshLibrary::getAll();
    if (ImGui::TreeNode("Meshes")) {
        for (auto& [name, mesh] : meshes) {
            if (ImGui::Selectable(name.c_str())) {
                // Optionally assign selected mesh to the selected entity
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();
}
void Application::drawScenePanel() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.2f, displaySize.y * 0.7f));

    ImGui::Begin("Scene Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    auto view = m_Scene->getRegistry().view<ZEN::TagComp>();

    if (ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Add Empty Entity")) {
            entt::entity entity = m_Scene->createEntity();
            m_Scene->getRegistry().emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Empty Entity"});
            m_SelectedEntity = entity;
        }
        if (ImGui::MenuItem("Add Cube")) {
            entt::entity entity = m_Scene->createEntity();
            m_Scene->getRegistry().emplace<ZEN::TagComp>(entity, ZEN::TagComp{.tag = "Cube"});
            m_SelectedEntity = entity;

            m_Scene->getRegistry().emplace<ZEN::TransformComp>(entity);
            m_Scene->getRegistry().emplace<ZEN::MeshComp>(entity, *ZEN::MeshLibrary::get("Cube"));
            m_Scene->getRegistry().emplace<ZEN::MaterialComp>(entity, ZEN::MaterialComp{.shaderID = 1, .textureID = 1, .specularTexID = 2});

        }
        ImGui::EndPopup();
    }

    for (auto entity : view) {
        auto &name = view.get<ZEN::TagComp>(entity);

        ImGuiTreeNodeFlags flags =
            (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_Leaf;

        bool opened = ImGui::TreeNodeEx((void *)(intptr_t)entity, flags, "%s", name.tag.c_str());

        if (ImGui::IsItemClicked())
            m_SelectedEntity = entity;

        if (opened)
            ImGui::TreePop();
    }

    ImGui::End();
}

void Application::onRender() {
    m_Renderer->beginFrame();
    m_RenderSystem->onRender(m_Scene->getRegistry());
    m_Renderer->bindDefaultFBO();
    onUIRender();
    m_Renderer->endFrame();
}






