#pragma once
#include "ImGUILayer.h"
#include "GLFW/glfw3.h"

namespace ZED {
    class ImGUILayerOpenGL : public ImGUILayer {
    public:
        void Init(const ImGuiCreateInfo &createInfo) override;

        ~ImGUILayerOpenGL() override;

        void BeginFrame() override;

        void Render() override;

        //command buffer ignored
        void EndFrame(void *commandBuffer) override;
    };
}
