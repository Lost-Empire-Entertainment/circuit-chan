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

#include "gameobjects/cube.hpp"
#include "graphics/texture.hpp"
#include "graphics/render.hpp"

using KalaWindow::Graphics::Window;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Graphics::OpenGL::Shader_OpenGL;

using CircuitGame::Graphics::Texture;
using CircuitGame::Graphics::Render;

using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::exists;
using std::string;
using std::to_string;
using std::ofstream;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using glm::translate;

static unsigned int VAO{};
static unsigned int VBO{};
static unsigned int EBO{};
static Window* mainWindow{};

static void CreateCube();

namespace CircuitGame::GameObjects
{
	Cube* Cube::Initialize(
		const string& name,
		Shader_OpenGL* shader,
		const vec3& pos,
		const vec3& rot,
		const vec3& scale)
	{
		Logger::Print(
			"Creating cube '" + name + "'.",
			"GAMEOBJECT",
			LogType::LOG_INFO);

		CreateCube();

		unique_ptr<Cube> newCube = make_unique<Cube>();
		newCube->SetName(name);
		newCube->SetPos(pos);
		newCube->SetRot(rot);
		newCube->SetScale(scale);
		newCube->SetShader(shader);
		newCube->SetUpdate(true);

		Render::createdCubes[name] = move(newCube);
		Render::runtimeCubes.push_back(Render::createdCubes[name].get());

		Logger::Print(
			"Initialized gameobject '" + name + "'!",
			"GAMEOBJECT",
			LogType::LOG_SUCCESS);
		return Render::createdCubes[name].get();
	}

	bool Cube::Render()
	{
		if (!CanUpdate()) return false;

		string name = GetName();
		const Texture* tex = GetTexture();

		if (tex == nullptr)
		{
			Logger::Print(
				"Cannot render gameobject '" + name + "' because its texture is nullptr!",
				"GAMEOBJECT",
				LogType::LOG_ERROR,
				2);

			return false;
		}
		
		const Shader_OpenGL* shader = GetShader();
		if (shader == nullptr)
		{
			Logger::Print(
				"Cannot render gameobject '" + name + "' because its shader is nullptr!",
				"GAMEOBJECT",
				LogType::LOG_ERROR,
				2);

			return false;
		}

		vec3 pos = GetPos();
		string posString = 
			to_string(pos.x) + ", " +
			to_string(pos.y) + ", " +
			to_string(pos.z);
		Logger::Print("Position: " + posString);

		if (!shader->Bind()) return false;
		
		unsigned int id = tex->GetTextureID();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);

		mat4 model = mat4(1.0f);
		model = translate(model, vec3(0.0f, 1.0f, 0.0f));

		shader->SetMat4(id, "model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		return true;
	}

	Cube::~Cube()
	{
		if (VAO)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		if (VBO)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (EBO)
		{
			glDeleteBuffers(1, &EBO);
			EBO = 0;
		}

		Logger::Print(
			"Destroyed gameobject '" + GetName() + "'!",
			"GAMEOBJECT",
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