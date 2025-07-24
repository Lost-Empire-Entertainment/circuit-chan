//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <filesystem>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "glm/gtc/matrix_transform.hpp"

//kalawindow
#include "graphics/window.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_opengl.hpp"
#include "graphics/opengl/opengl_core.hpp"

#include "graphics/cube.hpp"
#include "graphics/texture.hpp"

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

using KalaTestProject::Graphics::Texture;

using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::exists;
using std::string;
using std::ofstream;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using glm::translate;

static unsigned int VAO{};
static unsigned int VBO{};
static unsigned int EBO{};
static Window* mainWindow{};

static Shader_OpenGL* thisCubeShader{};

static void CreateCube();

namespace KalaTestProject::Graphics
{
	bool Cube::Initialize()
	{
		string vertPath = path(current_path() / "files" / "shaders" / "cube.vert").string();
		string fragPath = path(current_path() / "files" / "shaders" / "cube.frag").string();

		mainWindow = Window::windows.front();

		string shaderName = "shader_cube";

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
		thisCubeShader = Shader_OpenGL::CreateShader(
			shaderName,
			stages,
			mainWindow);

		if (thisCubeShader == nullptr) return false;

		CreateCube();

		string textureName = "cubeTexture";
		string texturePath = path(current_path() / "files" / "textures" / "cube.png").string();
		Texture* tex = Texture::CreateTexture(
			textureName,
			texturePath);

		Logger::Print(
			"Initialized cube!",
			"TRIANGLE_OPENGL",
			LogType::LOG_SUCCESS);

		SetCubeShader(thisCubeShader);
		
		return true;
	}

	void Cube::Render()
	{
		if (thisCubeShader == nullptr)
		{
			thisCubeShader = GetCubeShader();
		}
		if (thisCubeShader == nullptr)
		{
			Logger::Print(
				"Cube shader is nullptr!",
				"CUBE",
				LogType::LOG_ERROR,
				2);
			return;
		}

		unsigned int thisID = Texture::createdTextures["shader_cube"]->GetTextureID();
		thisCubeShader->Bind();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, thisID);

		mat4 model = mat4(1.0f);
		model = translate(model, vec3(0.0f, 1.0f, 0.0f));

		thisCubeShader->SetMat4(thisID, "model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void Cube::Destroy()
	{
		if (GetCubeShader() == nullptr) return;
		if (thisCubeShader == nullptr)
		{
			thisCubeShader = GetCubeShader();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		Logger::Print(
			"Destroyed cube!",
			"CUBE",
			LogType::LOG_SUCCESS);
	}
}

void CreateCube()
{
	float vertices[] =
	{
		//positions          //normals           //texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

	//position
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(float),
		(void*)(6 * sizeof(float)));



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}