#pragma once
#include <imgui_impl_glfw.h>
#include <memory>
#include <variant>
#include <vulkan/vulkan.hpp>
#include <ZeusEngineCore/IRenderer.h>
#include <ZeusEngineCore/InfoVariants.h>

enum class RendererAPI;



struct ImGuiCreateInfo {
    GLFWwindow* window{};
    RendererAPI api;

    //Vulkan data if specified
    std::variant<std::monostate, VulkanContextInfo, OpenGLContextInfo> backendData;
};

class ImGUILayer {
public:
    std::function<void(vk::CommandBuffer)> callback = nullptr;

    virtual ~ImGUILayer() = default;
    virtual void Init(const ImGuiCreateInfo& createInfo) = 0;
    virtual void BeginFrame() = 0;
    virtual void Render() = 0;
    virtual void EndFrame(void* commandBuffer) = 0;

    static std::unique_ptr<ImGUILayer> Create(RendererAPI api);
};
