//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include "graphics/opengl/shader_opengl.hpp"

namespace KalaTestProject::Graphics
{
	using KalaWindow::Graphics::OpenGL::Shader_OpenGL;

	class Triangle_OpenGL
	{
	public:
		static bool Initialize();

		static void SetTriangleShader(Shader_OpenGL* newTriangleShader)
		{
			triangleShader = newTriangleShader;
		}
		static Shader_OpenGL* GetTriangleShader() { return triangleShader; }

		static void Render();
		static void Destroy();
	private:
		static inline Shader_OpenGL* triangleShader{};
	};
}