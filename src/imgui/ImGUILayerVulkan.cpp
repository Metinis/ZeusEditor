#include "ImGUILayerVulkan.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>

void ImGUILayerVulkan::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(window, true);
    //TODO finish vulkan init
}
void ImGUILayerVulkan::Shutdown() {
    //ImGui_ImplVulkan_Shutdown(); //TODO fix this

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void ImGUILayerVulkan::BeginFrame() {
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
}
void ImGUILayerVulkan::Render() {


    ImGui::ShowDemoWindow();

    ImGui::Render();
}