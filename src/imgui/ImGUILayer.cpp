#include "ImGUILayer.h"

#include "ImGUILayerOpenGL.h"
#include "ImGUILayerVulkan.h"
#include <ZeusEngineCore/RendererAPI.h>

using namespace ZED;
std::unique_ptr<ImGUILayer> ImGUILayer::Create(const ZEN::RendererAPI api) {
    switch (api) {
        case ZEN::RendererAPI::OpenGL: return std::make_unique<ImGUILayerOpenGL>();
        case ZEN::RendererAPI::Vulkan: return std::make_unique<ImGUILayerVulkan>();
        default: return nullptr;
    }
}
