#include "Application.h"
#include "src/config.h"
#include "ZeusEngineCore/Vertex.h"
#include <iostream>
#include <memory>

using namespace ZED;
Application::Application(ZEN::eRendererAPI api) : m_API(api) {
    m_Window = std::make_unique<ZEN::Window>(1280, 720, "Zeus Editor", m_API);
}
Application::~Application() {
     
}

void Application::run() {
    while(m_Running && !m_Window->shouldClose()) {
        m_Window->pollEvents();

        const float dt = m_Window->getDeltaTime();
        update(dt);
        render();
    }
}
void Application::processEvents() {
    
}
void Application::update(float deltaTime) {
    
}

void Application::render() {
    
}






