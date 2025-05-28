#pragma once
#include "ImGUILayer.h"
#include "GLFW/glfw3.h"

class ImGUILayerOpenGL : public ImGUILayer{
public:
    void Init(GLFWwindow* window) override;
    ~ImGUILayerOpenGL() override;
    void BeginFrame() override;
    void Render() override;
};
