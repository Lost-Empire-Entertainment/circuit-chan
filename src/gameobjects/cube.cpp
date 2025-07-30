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
#include "glm/gtc/quaternion.hpp"

//kalawindow
#include "graphics/window.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_opengl.hpp"
#include "graphics/opengl/opengl_core.hpp"
#include "graphics/texture.hpp"
#include "core/containers.hpp"

#include "gameobjects/cube.hpp"
#include "core/gamecore.hpp"

using KalaWindow::Graphics::Window;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Graphics::OpenGL::Shader_OpenGL;
using KalaWindow::Graphics::Texture;
using KalaWindow::Core::globalID;

using CircuitGame::Core::createdCubes;
using CircuitGame::Core::runtimeCubes;
using CircuitGame::GameObjects::Cube;

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
using glm::radians;
using glm::quat;
using glm::mat4_cast;
using glm::scale;

static void CreateCube(Cube* cube);

namespace CircuitGame::GameObjects
{
	Cube* Cube::Initialize(
		const string& name,
		const vec3& pos,
		const vec3& rot,
		const vec3& scale)
	{
		Logger::Print(
			"Creating gameobject '" + name + "'.",
			"GAMEOBJECT",
			LogType::LOG_DEBUG);

		u32 newID = globalID++;
		unique_ptr<Cube> newCube = make_unique<Cube>();
		Cube* cubePtr = newCube.get();

		CreateCube(cubePtr);

		newCube->SetName(name);
		newCube->SetID(newID);
		newCube->SetPos(pos);
		newCube->SetRot(rot);
		newCube->SetScale(scale);
		newCube->SetUpdate(true);

		createdCubes[newID] = move(newCube);
		runtimeCubes.push_back(cubePtr);

		Logger::Print(
			"Created gameobject '" + name + "'!",
			"GAMEOBJECT",
			LogType::LOG_SUCCESS);

		return cubePtr;
	}

	bool Cube::Render(
		const mat4& view,
		const mat4& projection)
	{
		if (!CanUpdate()) return false;

		string name = GetName();
		
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

		if (!shader->Bind())
		{
			Logger::Print(
				"Failed to bind shader '" + shader->GetName() + "'!",
				"GAMEOBJECT_CUBE",
				LogType::LOG_ERROR,
				2);

			return false;
		}

		u32 programID = shader->GetProgramID();

		shader->SetMat4(programID, "projection", projection);
		shader->SetMat4(programID, "view", view);

		mat4 model = mat4(1.0f);
		model = translate(model, GetPos());
		quat newRot = quat(radians(GetRot()));
		model *= mat4_cast(newRot);
		model = glm::scale(model, vec3(1));

		shader->SetMat4(programID, "model", model);

		u32 vao = GetVAO();
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, 24);

		return true;
	}

	Cube::~Cube()
	{
		u32 vao = GetVAO();
		u32 vbo = GetVBO();
		u32 ebo = GetEBO();

		if (vao != 0)
		{
			glDeleteVertexArrays(1, &vao);
			SetVAO(0);
		}
		if (vbo != 0)
		{
			glDeleteBuffers(1, &vbo);
			SetVBO(0);
		}
		if (ebo != 0)
		{
			glDeleteBuffers(1, &ebo);
			SetEBO(0);
		}

		Logger::Print(
			"Destroyed gameobject '" + GetName() + "'!",
			"GAMEOBJECT",
			LogType::LOG_SUCCESS);
	}
}

void CreateCube(Cube* cube)
{
	f32 vertices[] =
	{
		//edges of the cube
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,

		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,

		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		//connecting edges
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,

		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,

		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
	};

	u32 vao{};
	u32 vbo{};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
		3 * sizeof(f32),
		(void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cube->SetVAO(vao);
	cube->SetVBO(vbo);
}