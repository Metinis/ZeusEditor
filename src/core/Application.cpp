#include "Application.h"
#include "src/config.h"
#include <memory>
#include <string>
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/RenderSystem.h>
#include <ZeusEngineCore/CameraSystem.h>
#include <ZeusEngineCore/ModelLibrary.h>
#include <ZeusEngineCore/ModelImporter.h>

using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {
    m_Scene = std::make_unique<ZEN::Scene>();
    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", m_API,
        m_Scene->getDispatcher());
    m_Renderer = std::make_unique<ZEN::Renderer>(m_API, m_Window->getNativeWindow(), m_Scene->getDispatcher());

    std::string resourceRoot = RESOURCE_ROOT;
    m_Renderer->createDefaultShader("/shaders/glbasic4.1.vert", "/shaders/glbasic4.1.frag", resourceRoot);
    m_ModelImporter = std::make_unique<ZEN::ModelImporter>(m_Scene.get(), m_Renderer->getResourceManager());
    m_ModelLibrary = std::make_unique<ZEN::ModelLibrary>(m_Renderer->getResourceManager());

    m_RenderSystem = std::make_unique<ZEN::RenderSystem>(m_Renderer.get(), m_Scene.get());
    m_CameraSystem = std::make_unique<ZEN::CameraSystem>(m_Scene.get());

    m_ImGuiLayer = ImGUILayer::create(m_Window->getNativeWindow(), api);
    m_InspectorPanel = std::make_unique<InspectorPanel>(m_Scene.get(), m_ModelLibrary.get());
    m_ProjectPanel = std::make_unique<ProjectPanel>(m_Scene.get(), m_ModelLibrary.get());
    m_ViewPanel = std::make_unique<ViewPanel>(m_Scene.get(), m_Renderer->getColorTextureHandle());
    m_ScenePanel = std::make_unique<ScenePanel>(m_Scene.get(), m_ModelLibrary.get());

    m_Running = true;

    m_Scene->createDefaultScene(resourceRoot, m_Renderer.get(), m_ModelLibrary.get(), m_ModelImporter.get());


}
Application::~Application() = default;

void Application::run() {
    while(m_Running && !m_Window->shouldClose()) {
        m_Window->pollEvents();

        const float dt = m_Window->getDeltaTime();
        onUpdate(dt);
        onRender();
    }
}

void Application::onUpdate(float deltaTime) {
    m_CameraSystem->onUpdate(deltaTime);
    m_RenderSystem->onUpdate();
}


void Application::onUIRender() {
    m_ImGuiLayer->beginFrame();

    m_ScenePanel->onImGuiRender();
    m_ViewPanel->onImGuiRender();
    m_InspectorPanel->onImGuiRender();
    m_ProjectPanel->onImGuiRender();

    m_ImGuiLayer->render();
    m_ImGuiLayer->endFrame(nullptr);
}

void Application::onRender() {
    m_Renderer->beginFrame();
    m_RenderSystem->onRender();
    m_Renderer->bindDefaultFBO();
    onUIRender();
    m_Renderer->endFrame();
}






