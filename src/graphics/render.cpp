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
#include "graphics/opengl/shader_opengl.hpp"
#include "core/log.hpp"
#include "core/core.hpp"

//glm
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "graphics/render.hpp"
#include "graphics/texture.hpp"
#include "gameobjects/cube.hpp"

//kalawindow
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::OpenGL::Renderer_OpenGL;
using KalaWindow::Graphics::OpenGL::OpenGLCore;
using KalaWindow::Graphics::OpenGL::Shader_OpenGL;
using KalaWindow::Graphics::OpenGL::ShaderStage;
using KalaWindow::Graphics::OpenGL::ShaderType;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Core::KalaWindowCore;

using CircuitGame::GameObjects::GameObjectType;
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

static vec2 lastSize{};

static Window* mainWindow{};

struct TextureData
{
	string textureName;
	string texturePath;
};
struct ShaderData
{
	string shaderName;
	string vertPath;
	string fragPath;
};
struct GameObjectData
{
	string name;
	GameObjectType type;
	Texture* texture;
	Shader_OpenGL* shader;
};

static bool InitializeTextures(const vector<TextureData>& textures);
static bool InitializeShaders(const vector<ShaderData>& shaders);
static bool CreateGameObjects(const vector<GameObjectData>& gameObjects);

static void ResizeProjectionMatrix();

namespace CircuitGame::Graphics
{
	bool Render::Initialize()
	{
		mainWindow = Window::runtimeWindows.front();

		if (!Renderer_OpenGL::Initialize(mainWindow)) return false;

		mainWindow->SetRedrawCallback(Redraw);

		mainWindow->SetResizeCallback(ResizeProjectionMatrix);
		ResizeProjectionMatrix();

		vector<TextureData> textures{};
		TextureData textureData =
		{
			.textureName = "texture_cube",
			.texturePath = path(current_path() / "files" / "textures" / "cube.jpg").string()
		};
		textures.push_back(textureData);
		if (!InitializeTextures(textures)) return false;

		vector<ShaderData> shaders{};
		ShaderData shaderData =
		{
			.shaderName = "shader_cube",
			.vertPath = path(current_path() / "files" / "shaders" / "cube.vert").string(),
			.fragPath = path(current_path() / "files" / "shaders" / "cube.frag").string()
		};
		shaders.push_back(shaderData);
		if (!InitializeShaders(shaders)) return false;

		vector<GameObjectData> gameObjects{};

		GameObjectData cubeData =
		{
			.name = "cube_1",
			.type = GameObjectType::cube,
			.texture = Render::createdTextures["texture_cube"].get(),
			.shader = Shader_OpenGL::createdShaders["shader_cube"].get()
		};
		gameObjects.push_back(cubeData);
		CreateGameObjects(gameObjects);

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

		for (const auto& object : runtimeCubes)
		{
			object->Render();
		}

		Renderer_OpenGL::SwapOpenGLBuffers(mainWindow);
	}

	void Render::Shutdown()
	{
		for (const auto& obj : runtimeCubes)
		{
			obj->SetUpdate(false);
		}

		createdTextures.clear();
		createdCubes.clear();
	}
}

bool InitializeTextures(const vector<TextureData>& textures)
{
	for (const auto& texture : textures)
	{
		string textureName = texture.textureName;
		string texturePath = texture.texturePath;

		Texture* tex = Texture::CreateTexture(
			textureName,
			texturePath);
	}

	return true;
}

bool InitializeShaders(const vector<ShaderData>& shaders)
{
	for (const auto& shader : shaders)
	{
		string shaderName = shader.shaderName;

		string vertPath = shader.vertPath;
		string fragPath = shader.fragPath;
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
	}

	return true;
}

bool CreateGameObjects(const vector<GameObjectData>& gameObjects)
{
	for (const auto& obj : gameObjects)
	{
		Cube* cube = Cube::Initialize(
			obj.name,
			obj.shader,
			vec3(-5, 0, 0));

		cube->SetTexture(obj.texture);
	}

	return true;
}

void ResizeProjectionMatrix()
{
	Shader_OpenGL* shader = Shader_OpenGL::createdShaders["shdader_cube"].get();
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
}