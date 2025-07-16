#include "ImGUILayer.h"
#include "ImGUILayerOpenGL.h"
#include "ImGUILayerVulkan.h"
#include "../../../ZeusEngineCore/src/renderer/OpenGL/APIBackend.h"
#include <ZeusEngineCore/IRendererBackend.h>

using namespace ZED;
std::unique_ptr<ImGUILayer> ImGUILayer::Create(GLFWwindow* window, ZEN::IRendererBackend* apiBackend) {
    switch (apiBackend->GetAPI()) {
        case ZEN::eRendererAPI::OpenGL: {
            auto* backendAPI = dynamic_cast<ZEN::OGLAPI::APIBackend*>(apiBackend);
            ZEN::OGLAPI::BackendInfo backendInfo = backendAPI->GetInfo();
            return std::make_unique<ImGUILayerOpenGL>(window, backendInfo);
        }
        case ZEN::eRendererAPI::Vulkan:{
            auto* backendAPI = dynamic_cast<ZEN::VKAPI::APIBackend*>(apiBackend);
            ZEN::VKAPI::BackendInfo backendInfo = backendAPI->GetInfo();
            return std::make_unique<ImGUILayerVulkan>(window, backendInfo);
        }
        default: return nullptr;
    }
}
