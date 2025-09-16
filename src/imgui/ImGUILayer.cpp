#include "ImGUILayer.h"
#include "ImGUILayerOpenGL.h"
#include "ImGUILayerVulkan.h"

using namespace ZED;
std::unique_ptr<ImGUILayer> ImGUILayer::create(GLFWwindow* window, ZEN::eRendererAPI api) {
    switch (api) {
        case ZEN::eRendererAPI::OpenGL: {
            return std::make_unique<ImGUILayerOpenGL>(window);
        }
        case ZEN::eRendererAPI::Vulkan:{
            return std::make_unique<ImGUILayerVulkan>(window);
        }
        default: return nullptr;
    }
}
