#include "ImGUILayerOpenGL.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace ZED;
ImGUILayerOpenGL::ImGUILayerOpenGL(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __APPLE__
    ImGui_ImplOpenGL3_Init("#version 410");
#else
    ImGui_ImplOpenGL3_Init("#version 450");
#endif
    callback = [this](void* cmd) {
        this->endFrame(cmd);
    };
}

ImGUILayerOpenGL::~ImGUILayerOpenGL() {
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void ImGUILayerOpenGL::beginFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}
void ImGUILayerOpenGL::render() {
    ImGui::Render();
}
// commandBuffer ignored here
void ImGUILayerOpenGL::endFrame(void* commandBuffer) {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

