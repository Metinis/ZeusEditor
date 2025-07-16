#pragma once
#include <imgui_impl_glfw.h>
#include <memory>
#include <functional>

namespace ZEN {
    class IRendererBackend;
}
namespace ZED {
    class ImGUILayer {
    public:
        std::function<void(void*)> callback = nullptr;

        virtual ~ImGUILayer() = default;

        virtual void BeginFrame() = 0;

        virtual void Render() = 0;

        virtual void EndFrame(void *commandBuffer) = 0;

        static std::unique_ptr<ImGUILayer> Create(GLFWwindow* window, ZEN::IRendererBackend* apiBackend);
    };
}
