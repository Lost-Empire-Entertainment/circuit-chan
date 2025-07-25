//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <string>
#include <vector>
#include <filesystem>

//kalawindow
#include "graphics/window.hpp"
#include "graphics/opengl/opengl.hpp"
#include "graphics/opengl/opengl_core.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_opengl.hpp"

//glm
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "graphics/render.hpp"
#include "graphics/texture.hpp"
#include "gameobjects/cube.hpp"

//kalawindow
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::OpenGL::Renderer_OpenGL;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Graphics::OpenGL::OpenGLCore;
using KalaWindow::Graphics::OpenGL::Shader_OpenGL;
using KalaWindow::Graphics::OpenGL::ShaderStage;
using KalaWindow::Graphics::OpenGL::ShaderType;

using CircuitGame::GameObjects::Cube;
using CircuitGame::Graphics::Texture;
using CircuitGame::Graphics::Render;

using glm::ortho;
using glm::perspective;
using glm::value_ptr;
using std::string;
using std::vector;
using std::filesystem::path;
using std::filesystem::current_path;
using std::pair;

static vec2 lastSize{};

static Window* mainWindow{};

static bool InitializeTextures(vector<pair<string, string>> textures);
static bool InitializeShaders(vector<pair<string, pair<string, string>>> shaders);

static void ResizeProjectionMatrix();

namespace CircuitGame::Graphics
{
	bool Render::Initialize()
	{
		mainWindow = Window::windows.front();

		if (!Renderer_OpenGL::Initialize(mainWindow)) return false;

		mainWindow->SetRedrawCallback(Redraw);

		mainWindow->SetResizeCallback(ResizeProjectionMatrix);
		ResizeProjectionMatrix();

		//if (!InitializeTextures()) return false;
		//if (!InitializeShaders()) return false;

		Cube* cube = new Cube();
		if (cube->Initialize(
			"cube01",
			Shader_OpenGL::createdShaders["cubeShader"].get()) == nullptr)
		{
			return false;
		}
		Texture* tex = createdTextures["cubeTexture"].get();
		if (tex == nullptr)
		{
			return false;
		}
		cube->SetTexture(tex);

		return true;
	}

	void Render::Update()
	{
		if (!mainWindow->IsIdle()) Redraw();
	}

	void Render::Redraw()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //dark gray
		glClear(GL_COLOR_BUFFER_BIT);

		//TODO: FIX THIS -> Cube::Render();

		Renderer_OpenGL::SwapOpenGLBuffers(mainWindow);
	}

	void Render::Shutdown()
	{
		for (const auto& obj : runtimeGameObjects)
		{
			obj->SetUpdate(false);
		}

		createdTextures.clear();
		createdGameObjects.clear();
	}
}

bool InitializeTextures(vector<pair<string, string>> textures)
{
	auto CreateTexture = [](pair<string, string> textureValues) -> bool
		{
			string textureName = textureValues.first;
			string texturePath = textureValues.second;

			Texture* tex = Texture::CreateTexture(
				textureName,
				texturePath);

			if (!tex) return false;
			else
			{
				Render::runtimeTextures.push_back(tex);
				return true;
			}
		};

	for (const auto& texture : textures)
	{
		if (!CreateTexture(texture)) return false;
	}

	return true;
}

bool InitializeShaders(vector<pair<string, pair<string, string>>> shaders)
{
	auto CreateShader = [](pair<string, pair<string, string>> shaderValues) -> bool
		{
			string shaderName = shaderValues.first;

			string vertPath = shaderValues.second.first;
			string fragPath = shaderValues.second.second;
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

			Shader_OpenGL* shader = Shader_OpenGL::CreateShader(
				shaderName,
				stages,
				mainWindow);

			return shader;
		};

	for (const auto& shader : shaders)
	{
		if (!CreateShader(shader)) return false;
	}

	return true;
}

void ResizeProjectionMatrix()
{
	/*
	
	//TODO: FIX THIS

	Shader_OpenGL* shader = Cube::GetCubeShader();
	if (!shader) return;

	vec2 framebufferSize = mainWindow->GetSize();
	float aspect = framebufferSize.x / framebufferSize.y;

	float orthoWidth = 1.0f;
	float orthoHeight = 1.0f;

	if (aspect >= 1.0f) orthoWidth = aspect;
	else orthoHeight = 1.0f / aspect;

	mat4 proj = ortho(
		-orthoWidth, orthoWidth,
		-orthoHeight, orthoHeight,
		-1.0f, 1.0f);

	mat4 model = mat4(1.0f);

	GLuint program = shader->GetProgramID();
	glUseProgram(program);

	//upload projection

	int projLoc = glGetUniformLocation(program, "uProjection");

	//Logger::Print(
	//	  "uProjection uniform location: " + to_string(projLoc),
	//	  "Render",
	//	  LogType::LOG_DEBUG);

	if (projLoc >= 0)
	{
		glUniformMatrix4fv(
			projLoc,
			1,
			GL_FALSE,
			value_ptr(proj));
	}

	//upload model

	int modelLoc = glGetUniformLocation(program, "uModel");
	
	//Logger::Print(
	//	  "uModel uniform location: " + to_string(modelLoc),
	//	  "Render",
	//	  LogType::LOG_DEBUG);

	if (modelLoc >= 0)
	{
		glUniformMatrix4fv(
			modelLoc,
			1,
			GL_FALSE,
			value_ptr(model));
	}
	*/
}