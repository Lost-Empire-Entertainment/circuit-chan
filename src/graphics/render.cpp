//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <string>
#include <vector>
#include <filesystem>
#include <memory>

//kalawindow
#include "graphics/window.hpp"
#include "graphics/texture.hpp"
#include "graphics/opengl/opengl.hpp"
#include "graphics/opengl/opengl_core.hpp"
#include "graphics/opengl/shader_opengl.hpp"
#include "core/log.hpp"
#include "core/core.hpp"
#include "core/containers.hpp"

//glm
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "graphics/render.hpp"
#include "gameobjects/gameobject.hpp"
#include "gameobjects/cube.hpp"
#include "core/gamecore.hpp"

//kalawindow
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::Texture;
using KalaWindow::Graphics::OpenGL::Renderer_OpenGL;
using KalaWindow::Graphics::OpenGL::OpenGLCore;
using KalaWindow::Graphics::OpenGL::Shader_OpenGL;
using KalaWindow::Graphics::OpenGL::ShaderStage;
using KalaWindow::Graphics::OpenGL::ShaderType;
using KalaWindow::Graphics::OpenGL::Texture_OpenGL;
using KalaWindow::Graphics::TextureType;
using KalaWindow::Graphics::TextureFormat;
using KalaWindow::Graphics::TextureUsage;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Core::KalaWindowCore;
using KalaWindow::Core::createdOpenGLTextures;
using KalaWindow::Core::createdOpenGLShaders;
using KalaWindow::Core::runtimeOpenGLTextures;
using KalaWindow::Core::runtimeOpenGLShaders;

using CircuitGame::GameObjects::GameObject;
using CircuitGame::GameObjects::GameObjectType;
using CircuitGame::GameObjects::Cube;
using CircuitGame::Core::createdCubes;
using CircuitGame::Core::runtimeCubes;
using CircuitGame::Core::mainWindow;
using CircuitGame::Core::createdCamera;

using glm::ortho;
using glm::perspective;
using glm::value_ptr;
using glm::radians;
using std::string;
using std::vector;
using std::filesystem::path;
using std::filesystem::current_path;
using std::unique_ptr;
using std::make_unique;

static vec2 lastSize{};

static Texture_OpenGL* texturePtr{};
static Shader_OpenGL* shaderPtr{};

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
	Texture_OpenGL* texture;
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
		if (!Renderer_OpenGL::Initialize(mainWindow)) return false;

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); //Ensures callbacks run immediately
		glDebugMessageCallback(DebugCallback, nullptr);
#endif

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
			.vertPath = path(current_path() / "files" / "shaders" / "basic.vert").string(),
			.fragPath = path(current_path() / "files" / "shaders" / "basic.frag").string()
		};
		shaders.push_back(shaderData);
		if (!InitializeShaders(shaders)) return false;

		vector<GameObjectData> gameObjects{};

		GameObjectData cubeData =
		{
			.name = "cube_1",
			.type = GameObjectType::cube,
			.texture = texturePtr,
			.shader = shaderPtr
		};
		gameObjects.push_back(cubeData);
		CreateGameObjects(gameObjects);

		if (Camera::CreateCamera(mainWindow) == nullptr)
		{
			KalaWindowCore::ForceClose(
				"Camera error",
				"Failed to create camera!");
		}

		return true;
	}

	void Render::Update()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //dark gray
		glClear(
			GL_COLOR_BUFFER_BIT
			| GL_DEPTH_BUFFER_BIT);

		if (!mainWindow->IsIdle()) Redraw();

		Renderer_OpenGL::SwapOpenGLBuffers(mainWindow);
	}

	void Render::Redraw()
	{
		mat4 projection{};
		mat4 view{};
		if (createdCamera != nullptr)
		{
			projection = perspective(
				radians(createdCamera->GetFOV()),
				createdCamera->GetAspectRatio(),
				createdCamera->GetNearClip(),
				createdCamera->GetFarClip());

			view = createdCamera->GetViewMatrix();
		}

		if (runtimeCubes.size() > 0)
		{
			for (const auto& object : runtimeCubes)
			{
				if (object == nullptr)
				{
					Logger::Print(
						"Failed to render a cube because it was nullptr!",
						"RENDER",
						LogType::LOG_DEBUG);

					continue;
				}
				object->Render(view, projection);
			}
		}
	}

	void Render::Shutdown()
	{
		for (const auto& obj : runtimeCubes)
		{
			obj->SetUpdate(false);
		}

		createdCubes.clear();
	}
}

bool InitializeTextures(const vector<TextureData>& textures)
{
	for (const auto& texture : textures)
	{
		string textureName = texture.textureName;
		string texturePath = texture.texturePath;

		texturePtr = Texture_OpenGL::LoadTexture(
			textureName,
			texturePath,
			TextureType::Type_2D,
			TextureFormat::Format_RGBA8,
			TextureUsage::Usage_None);

		if (texturePtr == nullptr)
		{
			KalaWindowCore::ForceClose(
				"Texture error",
				"Failed to create cube texture!");
		}
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

		shaderPtr = Shader_OpenGL::CreateShader(
			shaderName,
			stages,
			mainWindow);

		if (shaderPtr == nullptr)
		{
			KalaWindowCore::ForceClose(
				"Shader error",
				"Failed to create cube shader!");
		}
	}

	return true;
}

bool CreateGameObjects(const vector<GameObjectData>& gameObjects)
{
	for (const auto& obj : gameObjects)
	{
		Cube* cube = Cube::Initialize(
			"cube_1",
			vec3(0.0f, 0.0f, -5.0f));

		if (cube == nullptr)
		{
			KalaWindowCore::ForceClose(
				"GameObject error",
				"Failed to create cube!");
		}

		cube->SetTexture(obj.texture);
		cube->SetShader(obj.shader);
	}

	return true;
}

void ResizeProjectionMatrix()
{
	vec2 framebufferSize = mainWindow->GetSize();
	float aspect = framebufferSize.x / framebufferSize.y;

	if (createdCamera != nullptr)
	{
		createdCamera->SetAspectRatio(aspect);
	}

	if (shaderPtr != nullptr)
	{
		float orthoWidth = 1.0f;
		float orthoHeight = 1.0f;

		if (aspect >= 1.0f) orthoWidth = aspect;
		else orthoHeight = 1.0f / aspect;

		mat4 proj = ortho(
			-orthoWidth, orthoWidth,
			-orthoHeight, orthoHeight,
			-1.0f, 1.0f);

		mat4 model = mat4(1.0f);

		GLuint program = shaderPtr->GetProgramID();
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
}