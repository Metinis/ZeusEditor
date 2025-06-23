#pragma once
#include "ImGUILayer.h"
#include <ZeusEngineCore/ScopedWaiter.h>

class ImGUILayerVulkan : public ImGUILayer{
public:
    void Init(const ImGuiCreateInfo& createInfo) override;
    ~ImGUILayerVulkan() override;
    void BeginFrame() override;
    void Render() override;
    void EndFrame(void* commandBuffer) override;
private:
    vk::Device m_Device{};
};
