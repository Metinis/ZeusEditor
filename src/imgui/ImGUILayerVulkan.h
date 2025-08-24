#pragma once
#include "ImGUILayer.h"

namespace ZED {
    class ImGUILayerVulkan : public ImGUILayer {
    public:
        ImGUILayerVulkan(GLFWwindow *window);

        ~ImGUILayerVulkan() override;

        void beginFrame() override;

        void render() override;

        void endFrame(void *commandBuffer) override;
    };
}
