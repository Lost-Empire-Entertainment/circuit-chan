//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <filesystem>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

//kalawindow
#include "graphics/window.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_opengl.hpp"
#include "graphics/opengl/opengl_core.hpp"

#include "graphics/opengl/triangle_opengl.hpp"

using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::ShutdownState;
using KalaWindow::Graphics::PopupAction;
using KalaWindow::Graphics::PopupResult;
using KalaWindow::Graphics::PopupType;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Graphics::OpenGL::Shader_OpenGL;
using KalaWindow::Graphics::OpenGL::ShaderStage;
using KalaWindow::Graphics::OpenGL::ShaderType;

using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::exists;
using std::string;
using std::ofstream;
using std::unique_ptr;
using std::make_unique;
using std::vector;

static unsigned int VAO{};
static unsigned int VBO{};
static Window* mainWindow{};

static Shader_OpenGL* triangleShader{};

namespace KalaTestProject::Graphics::OpenGL
{
	bool Triangle_OpenGL::Initialize()
	{
		string vertPath = path(current_path() / "files" / "shaders" / "triangle_opengl.vert").string();
		string fragPath = path(current_path() / "files" / "shaders" / "triangle_opengl.frag").string();

		mainWindow = Window::windows.front();

		string shaderName = "shader_triangle";

		struct ShaderStage vertStage
		{
			.shaderType = ShaderType::Shader_Vertex,
			.shaderPath = vertPath
		};
		struct ShaderStage fragStage
		{
			.shaderType = ShaderType::Shader_Fragment,
			.shaderPath = fragPath
		};

		vector<ShaderStage> stages
		{
			vertStage,
			fragStage
		};
		Shader_OpenGL* shaderPointer = Shader_OpenGL::CreateShader(
			shaderName,
			stages,
			mainWindow);

		if (shaderPointer == nullptr) return false;

		float vertices[] = 
		{
			-0.5f, -0.5f, 0.0f,  // left
			 0.5f, -0.5f, 0.0f,  // right
			 0.0f,  0.5f, 0.0f   // top
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(vertices),
			vertices,
			GL_STATIC_DRAW);

		//layout(location = 0) in vec2 aPos;
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		Logger::Print(
			"Initialized triangle!",
			"TRIANGLE_OPENGL",
			LogType::LOG_SUCCESS);

		SetTriangleShader(shaderPointer);
		
		return true;
	}

	void Triangle_OpenGL::Render()
	{
		if (triangleShader == nullptr)
		{
			triangleShader = GetTriangleShader();
		}
		if (triangleShader == nullptr)
		{
			Logger::Print(
				"Triangle shader is nullptr!",
				"TRIANGLE_OPENGL",
				LogType::LOG_ERROR,
				2);
			return;
		}

		triangleShader->Bind();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Triangle_OpenGL::Destroy()
	{
		if (GetTriangleShader() == nullptr) return;
		if (triangleShader == nullptr)
		{
			triangleShader = GetTriangleShader();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		Logger::Print(
			"Destroyed triangle!",
			"TRIANGLE_OPENGL",
			LogType::LOG_SUCCESS);
	}
}