#include "ImGUILayerVulkan.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <ZeusEngineCore/EngineConstants.h>
#include <glm/vec4.hpp>
#include <glm/gtc/color_space.hpp>

using namespace ZED;

void ImGUILayerVulkan::Init(const ImGuiCreateInfo& createInfo) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	static auto const loadVkFunc = +[](char const* name, void* userData) {
		return VULKAN_HPP_DEFAULT_DISPATCHER.vkGetInstanceProcAddr(
			*static_cast<vk::Instance*>(userData), name);
		};

	if (!std::holds_alternative<ZEN::VKAPI::ContextInfo>(createInfo.backendData)) {
		throw std::runtime_error{ "Invalid Create Info Data For Vulkan!" };
	}
	const auto& context = std::get<ZEN::VKAPI::ContextInfo>(createInfo.backendData);
	auto instance = context.instance;
	ImGui_ImplVulkan_LoadFunctions(context.apiVersion, loadVkFunc, &instance);

	if (!ImGui_ImplGlfw_InitForVulkan(createInfo.window, true)) {
		throw std::runtime_error{ "Failed to initialize Dear ImGui" };
	}

	ImGui_ImplVulkan_InitInfo initInfo{};
	initInfo.ApiVersion = context.apiVersion;
	initInfo.Instance = context.instance;
	initInfo.PhysicalDevice = context.physicalDevice;
	initInfo.Device = context.device;
	initInfo.QueueFamily = context.queueFamily;
	initInfo.Queue = context.queue;
	initInfo.MinImageCount = 2;
	initInfo.ImageCount = static_cast<std::uint32_t>(ZEN::buffering_v);
	initInfo.MSAASamples =
		static_cast<VkSampleCountFlagBits>(context.samples);
	initInfo.DescriptorPoolSize = 32;

	auto pipelineRenderingCreateInfo = vk::PipelineRenderingCreateInfo{};
	pipelineRenderingCreateInfo.setColorAttachmentCount(1).setColorAttachmentFormats(context.colorFormat);

	initInfo.PipelineRenderingCreateInfo = pipelineRenderingCreateInfo;
	initInfo.UseDynamicRendering = true;

	m_Device = context.device;

	if (!ImGui_ImplVulkan_Init(&initInfo)) {
		throw std::runtime_error{ "Failed to initialize Dear ImGui" };
	}

	ImGui::StyleColorsDark();
	for (auto& colour : ImGui::GetStyle().Colors) {
		auto const linear = glm::convertSRGBToLinear(
			glm::vec4{ colour.x, colour.y, colour.z, colour.w });
		colour = ImVec4{ linear.x, linear.y, linear.z, linear.w };
	}
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.99f;
	//used to submit to renderer
	callback = [this](vk::CommandBuffer cmd) {
		this->EndFrame(cmd);
		};
}
ImGUILayerVulkan::~ImGUILayerVulkan() {
	m_Device.waitIdle();
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
void ImGUILayerVulkan::BeginFrame() {
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplVulkan_NewFrame();

	ImGui::NewFrame();
}
void ImGUILayerVulkan::Render() {
	ImGui::Render();
}

void ImGUILayerVulkan::EndFrame(void* commandBuffer)
{
	ImDrawData* data = ImGui::GetDrawData();
	if (data == nullptr) { return; }
	VkCommandBuffer vkCommandBuffer = reinterpret_cast<VkCommandBuffer>(commandBuffer);
	ImGui_ImplVulkan_RenderDrawData(data, vkCommandBuffer);
}
