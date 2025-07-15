#pragma once
#include <imgui_impl_glfw.h>
#include <memory>
#include <variant>
#include <vulkan/vulkan.hpp>
#include <ZeusEngineCore/Renderer.h>
#include <ZeusEngineCore/InfoVariants.h>

namespace ZED {
    struct ImGuiCreateInfo {
        GLFWwindow *window{};
        ZEN::RendererAPI api;

        //Vulkan data if specified
        ZEN::BackendContextVariant backendData;
    };

    class ImGUILayer {
    public:
        std::function<void(void*)> callback = nullptr;

        virtual ~ImGUILayer() = default;

        virtual void Init(const ImGuiCreateInfo &createInfo) = 0;

        virtual void BeginFrame() = 0;

        virtual void Render() = 0;

        virtual void EndFrame(void *commandBuffer) = 0;

        static std::unique_ptr<ImGUILayer> Create(ZEN::RendererAPI api);
    };
}
