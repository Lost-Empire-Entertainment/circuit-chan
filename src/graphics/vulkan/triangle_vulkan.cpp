//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <filesystem>
#include <string>
#include <vector>

//kalawindow
#include "core/log.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vulkan.hpp"
#include "graphics/vulkan/shader_vulkan.hpp"

#include "graphics/vulkan/triangle_vulkan.hpp"

//kalawindow
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::PopupAction;
using KalaWindow::Graphics::PopupResult;
using KalaWindow::Graphics::PopupType;
using KalaWindow::Graphics::ShutdownState;
using KalaWindow::Graphics::Vulkan::Renderer_Vulkan;
using KalaWindow::Graphics::Vulkan::Shader_Vulkan;
using KalaWindow::Graphics::Vulkan::ShaderType;
using KalaWindow::Graphics::Vulkan::ShaderStage;

using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::exists;
using std::string;
using std::vector;

static Window* mainWindow{};

static Shader_Vulkan* triangleShader{};

namespace KalaTestProject::Graphics::Vulkan
{
	bool Triangle_Vulkan::Initialize()
	{
		mainWindow = Window::windows.front();

		if (!Renderer_Vulkan::InitializeShaderSystem(mainWindow)) return false;

		string vertPath = path(current_path() / "files" / "shaders" / "triangle_vulkan.vert").string();
		string fragPath = path(current_path() / "files" / "shaders" / "triangle_vulkan.frag").string();

		string vertSPVPath = path(current_path() / "files" / "shaders" / "triangle_vulkan.vert.spv").string();
		string fragSPVPath = path(current_path() / "files" / "shaders" / "triangle_vulkan.frag.spv").string();

		vector<string> compileShaders = { vertPath, fragPath };
		vector<string> compileSPVShaders = { vertSPVPath, fragSPVPath };

		Shader_Vulkan::CompileShader(compileShaders, compileSPVShaders);

		string shaderName = "shader_triangle";

		struct ShaderStage vertStage
		{
			.shaderType = ShaderType::Shader_Vertex,
			.shaderPath = vertSPVPath
		};
		struct ShaderStage fragStage
		{
			.shaderType = ShaderType::Shader_Fragment,
			.shaderPath = fragSPVPath
		};

		vector<ShaderStage> stages
		{
			vertStage,
			fragStage
		};
		Shader_Vulkan* shaderPointer = Shader_Vulkan::CreateShader(
			shaderName,
			stages,
			mainWindow);

		if (shaderPointer == nullptr) return false;

		Logger::Print(
			"Initialized triangle!",
			"TRIANGLE_VULKAN",
			LogType::LOG_SUCCESS);

		SetTriangleShader(shaderPointer);

		return true;
	}

	void Triangle_Vulkan::Render()
	{
		if (triangleShader == nullptr)
		{
			triangleShader = GetTriangleShader();
		}
		if (triangleShader == nullptr)
		{
			Logger::Print(
				"Triangle shader is nullptr!",
				"TRIANGLE_VULKAN",
				LogType::LOG_ERROR,
				2);
			return;
		}
	}

	void Triangle_Vulkan::Destroy()
	{
		if (GetTriangleShader() == nullptr) return;
		if (triangleShader == nullptr)
		{
			triangleShader = GetTriangleShader();
		}

		//TODO: delete other remaining stuff here...

		Logger::Print(
			"Destroyed triangle!",
			"TRIANGLE_VULKAN",
			LogType::LOG_SUCCESS);
	}
}