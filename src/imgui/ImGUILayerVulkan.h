#pragma once
#include "ImGUILayer.h"
#include <ZeusEngineCore/ScopedWaiter.h>

namespace ZED {
    class ImGUILayerVulkan : public ImGUILayer {
    public:
        ImGUILayerVulkan(GLFWwindow* window, const ZEN::VKAPI::BackendInfo& backendInfo);

        ~ImGUILayerVulkan() override;

        void BeginFrame() override;

        void Render() override;

        void EndFrame(void *commandBuffer) override;

    private:
        vk::Device m_Device{};
    };
}
