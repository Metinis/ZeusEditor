#pragma once
#include "ImGUILayer.h"
#include "GLFW/glfw3.h"

class ImGUILayerVulkan : public ImGUILayer{
public:
    void Init(GLFWwindow* window) override;
    ~ImGUILayerVulkan() override;
    void BeginFrame() override;
    void Render() override;
};
