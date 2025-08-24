#pragma once
#include <imgui_impl_glfw.h>
#include <memory>
#include <functional>
#include <ZeusEngineCore/API.h>

namespace ZED {
    class ImGUILayer {
    public:
        std::function<void(void*)> callback = nullptr;

        virtual ~ImGUILayer() = default;

        virtual void beginFrame() = 0;

        virtual void render() = 0;

        virtual void endFrame(void *commandBuffer) = 0;

        static std::unique_ptr<ImGUILayer> create(GLFWwindow* window, ZEN::eRendererAPI api);
    };
}
