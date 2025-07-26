//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

//kalawindow
#include "graphics/opengl/shader_opengl.hpp"

#include "gameobjects/gameobject.hpp"
#include "graphics/texture.hpp"

namespace CircuitGame::GameObjects
{
	using std::string;

	//kalawindow
	using KalaWindow::Graphics::OpenGL::Shader_OpenGL;

	using CircuitGame::Graphics::Texture;

	class Cube : public GameObject
	{
	public:
		static Cube* Initialize(
			const string& name,
			Shader_OpenGL* shader,
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0),
			const vec3& scale = vec3(1));

		const Texture* GetTexture() { return texture; }
		void SetTexture(Texture* newTexture) { texture = newTexture; }

		bool Render() override;
		~Cube() override;
	private:
		Texture* texture{};
	};
}