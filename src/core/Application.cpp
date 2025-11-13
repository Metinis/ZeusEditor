#include "Application.h"
#include "src/config.h"
#include <memory>
#include <string>
#include <ZeusEngineCore/ZEngine.h>


using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {

    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", m_API);
    std::string resourceRoot = RESOURCE_ROOT;
    m_Engine = std::make_unique<ZEN::ZEngine>(m_API, m_Window->getNativeWindow(), resourceRoot);
    m_Window->attachDispatcher(m_Engine->getDispatcher());

    m_ImGuiLayer = ImGUILayer::create(m_Window->getNativeWindow(), api);

    m_MenuBarPanel = std::make_unique<MenuBarPanel>(m_Engine.get());
    m_InspectorPanel = std::make_unique<InspectorPanel>(m_Engine.get());
    m_ProjectPanel = std::make_unique<ProjectPanel>(m_Engine.get());
    m_ViewPanel = std::make_unique<ViewPanel>(m_Engine.get());
    m_ScenePanel = std::make_unique<ScenePanel>(m_Engine.get());

    m_Running = true;

    m_Engine->getScene().createDefaultScene(m_Engine.get());
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
    m_Engine->onUpdate(deltaTime);
}


void Application::onUIRender() {
    m_ImGuiLayer->beginFrame();

    m_MenuBarPanel->onImGuiRender();
    m_ScenePanel->onImGuiRender();
    m_ViewPanel->onImGuiRender();
    m_InspectorPanel->onImGuiRender();
    m_ProjectPanel->onImGuiRender();



    m_ImGuiLayer->render();
    m_ImGuiLayer->endFrame(nullptr);
}

void Application::onRender() {
    m_Engine->onRender([&](){onUIRender();});
}






