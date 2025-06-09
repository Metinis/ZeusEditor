#include "ImGUILayer.h"

#include "ImGUILayerOpenGL.h"
#include "ImGUILayerVulkan.h"
#include <ZeusEngineCore/RendererAPI.h>
std::unique_ptr<ImGUILayer> ImGUILayer::Create(const RendererAPI api) {
    switch (api) {
        case RendererAPI::OpenGL: return std::make_unique<ImGUILayerOpenGL>();
        case RendererAPI::Vulkan: return std::make_unique<ImGUILayerVulkan>();
        default: return nullptr;
    }
}
