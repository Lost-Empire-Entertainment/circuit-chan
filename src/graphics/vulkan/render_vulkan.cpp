//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//kalawindow
#include "graphics/window.hpp"
#include "graphics/vulkan/vulkan.hpp"
#include "graphics/vulkan/extensions_vulkan.hpp"
#include "core/log.hpp"

#include "core/core.hpp"
#include "graphics/vulkan/render_vulkan.hpp"
#include "graphics/vulkan/triangle_vulkan.hpp"

//kalawindow
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::Vulkan::Renderer_Vulkan;
using KalaWindow::Graphics::Vulkan::Extensions_Vulkan;
using KalaWindow::Graphics::Vulkan::VulkanLayers;
using KalaWindow::Graphics::Vulkan::VulkanInstanceExtensions;
using KalaWindow::Graphics::Vulkan::VulkanDeviceExtensions;
using KalaWindow::Graphics::Vulkan::UpdateResult;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Core::TimeFormat;
using KalaWindow::Core::DateFormat;

using KalaTestProject::Core::TestProject;
using KalaTestProject::Graphics::Vulkan::Render_Vulkan;

using std::chrono::steady_clock;

static Window* mainWindow{};

namespace KalaTestProject::Graphics::Vulkan
{
	bool Render_Vulkan::Initialize()
	{
		Logger::SetDefaultTimeFormat(TimeFormat::TIME_HMS_MS);

		mainWindow = Window::windows.front();

#ifdef _DEBUG
		Renderer_Vulkan::EnableLayer(VulkanLayers::V_IL_khronos_validation);
		Renderer_Vulkan::EnableInstanceExtension(VulkanInstanceExtensions::V_IE_ext_debug_utils);
#endif
		Renderer_Vulkan::EnableInstanceExtension(VulkanInstanceExtensions::V_IE_khr_surface);
		Renderer_Vulkan::EnableDeviceExtension(VulkanDeviceExtensions::V_DE_khr_swapchain);
		Renderer_Vulkan::EnableInstanceExtension(VulkanInstanceExtensions::V_IE_khr_win32_surface);

		if (!Renderer_Vulkan::Initialize(2)) return false;

		Extensions_Vulkan::CreateVulkanSurface(mainWindow);

		if (!Extensions_Vulkan::CreateSwapchain(mainWindow)) return false;

		if (!Renderer_Vulkan::CreateRenderPass(mainWindow)) return false;
		if (!Renderer_Vulkan::CreateFramebuffers(mainWindow)) return false;

		if (!Renderer_Vulkan::CreateCommandPool(mainWindow)) return false;
		if (!Renderer_Vulkan::CreateCommandBuffer(mainWindow)) return false;

		if (!Renderer_Vulkan::CreateSyncObjects(mainWindow)) return false;

		if (!Triangle_Vulkan::Initialize()) return false;

		return true;
	}

	void Render_Vulkan::Render()
	{
		if (!mainWindow->IsIdle()) Redraw();
	}

	void Render_Vulkan::Redraw()
	{
		uint32_t imageIndex = 0;
		UpdateResult result = Renderer_Vulkan::Update(mainWindow);

		if (result == UpdateResult::RESULT_RESIZE)
		{
			Renderer_Vulkan::HardReset(mainWindow);
			TestProject::lastFrameTime = steady_clock::now();
			return;
		}
		else if (result == UpdateResult::RESULT_ERROR)
		{
			Logger::Print(
				"Failed to begin frame! Skipping draw...",
				"RENDER_VULKAN",
				LogType::LOG_ERROR,
				2);
			TestProject::lastFrameTime = steady_clock::now();
			return;
		}
	}
}