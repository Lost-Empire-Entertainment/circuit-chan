//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include "graphics/opengl/shader_opengl.hpp"

namespace KalaTestProject::Graphics
{
	using KalaWindow::Graphics::OpenGL::Shader_OpenGL;

	class Cube
	{
	public:
		static bool Initialize();

		static void SetCubeShader(Shader_OpenGL* newCubeShader)
		{
			cubeShader = newCubeShader;
		}
		static Shader_OpenGL* GetCubeShader() { return cubeShader; }

		static void Render();
		static void Destroy();
	private:
		static inline Shader_OpenGL* cubeShader{};
	};
}