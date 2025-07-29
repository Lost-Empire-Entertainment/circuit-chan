//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

//kalawindow
#include "graphics/opengl/texture_opengl.hpp"

#include "gameobjects/gameobject.hpp"

namespace CircuitGame::GameObjects
{
	using std::string;

	//kalawindow
	using KalaWindow::Graphics::OpenGL::Texture_OpenGL;

	class Cube : public GameObject
	{
	public:
		virtual GameObject* Initialize(
			const string& name,
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0),
			const vec3& scale = vec3(1)) override;

		const Texture_OpenGL* GetTexture() { return texture; }
		void SetTexture(Texture_OpenGL* newTexture) { texture = newTexture; }

		bool Render() override;
		~Cube() override;
	private:
		Texture_OpenGL* texture{};
	};
}