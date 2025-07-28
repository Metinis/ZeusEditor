#pragma once
#include "ImGUILayer.h"

namespace ZEN::VKAPI{
    struct BackendInfo;
}
namespace ZED {
    class ImGUILayerVulkan : public ImGUILayer {
    public:
        ImGUILayerVulkan(GLFWwindow *window, const ZEN::VKAPI::BackendInfo &backendInfo);

        ~ImGUILayerVulkan() override;

        void BeginFrame() override;

        void Render() override;

        void EndFrame(void *commandBuffer) override;
    };
}
