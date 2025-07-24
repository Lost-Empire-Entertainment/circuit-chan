//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <string>

//kalawindow
#include "graphics/window.hpp"
#include "graphics/opengl/opengl.hpp"
#include "graphics/opengl/opengl_core.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_opengl.hpp"

//glm
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "graphics/render.hpp"
#include "graphics/triangle_opengl.hpp"

//kalawindow
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::OpenGL::Renderer_OpenGL;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Graphics::OpenGL::Shader_OpenGL;
using KalaWindow::Graphics::OpenGL::OpenGLCore;

using KalaTestProject::Graphics::Triangle_OpenGL;

using glm::mat4;
using glm::vec3;
using glm::ortho;
using glm::perspective;
using glm::value_ptr;

static kvec2 lastSize{};

static Window* mainWindow{};

static void ResizeProjectionMatrix();

namespace KalaTestProject::Graphics
{
	bool Render::Initialize()
	{
		mainWindow = Window::windows.front();

		if (!Renderer_OpenGL::Initialize(mainWindow)) return false;

		mainWindow->SetRedrawCallback(Redraw);

		if (!Triangle_OpenGL::Initialize()) return false;

		mainWindow->SetResizeCallback(ResizeProjectionMatrix);
		ResizeProjectionMatrix();

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

		Triangle_OpenGL::Render();

		Renderer_OpenGL::SwapOpenGLBuffers(mainWindow);
	}
}

void ResizeProjectionMatrix()
{
	Shader_OpenGL* shader = Triangle_OpenGL::GetTriangleShader();
	if (!shader) return;

	kvec2 framebufferSize = mainWindow->GetSize();
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
	/*
	Logger::Print(
		"uProjection uniform location: " + to_string(projLoc),
		"Render",
		LogType::LOG_DEBUG);
	*/

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
	/*
	Logger::Print(
		"uModel uniform location: " + to_string(modelLoc),
		"Render",
		LogType::LOG_DEBUG);
	*/

	if (modelLoc >= 0)
	{
		glUniformMatrix4fv(
			modelLoc,
			1,
			GL_FALSE,
			value_ptr(model));
	}
}