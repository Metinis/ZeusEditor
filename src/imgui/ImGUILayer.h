#pragma once
#include <imgui_impl_glfw.h>
#include <memory>

enum class RendererAPI;

class ImGUILayer {
public:
    virtual ~ImGUILayer() = default;
    virtual void Init(GLFWwindow* window) = 0;
    virtual void Shutdown() = 0;
    virtual void BeginFrame() = 0;
    virtual void Render() = 0;

    static std::unique_ptr<ImGUILayer> Create(RendererAPI api);
};
