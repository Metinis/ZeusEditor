#include "ImGUILayerOpenGL.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void ImGUILayerOpenGL::Init(const ImGuiCreateInfo& createInfo) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(createInfo.window, true);
#ifdef __APPLE__
    ImGui_ImplOpenGL3_Init("#version 410");
#else
    ImGui_ImplOpenGL3_Init("#version 450");
#endif
    callback = [this](vk::CommandBuffer cmd) {
        this->EndFrame(cmd);
    };
}
ImGUILayerOpenGL::~ImGUILayerOpenGL() {
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void ImGUILayerOpenGL::BeginFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}
void ImGUILayerOpenGL::Render() {
    ImGui::Render();
}
// commandBuffer ignored here
void ImGUILayerOpenGL::EndFrame(void* commandBuffer)
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
