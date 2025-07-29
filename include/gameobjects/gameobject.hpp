//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

#include "core/platform.hpp"
#include "graphics/opengl/shader_opengl.hpp"

namespace CircuitGame::GameObjects
{
	//TODO: ADD SETTER LIMITS TO GAMEOBJECT CLASSES
	//+ OTHERS WHERE NEEDED TO PREVENT MALFORMED VALUES AND CRASHES

	using KalaWindow::Graphics::OpenGL::Shader_OpenGL;

	using std::string;

	enum class GameObjectType
	{
		cube,
		pointLight,
		dirLight
	};

	class GameObject
	{
	public:
		bool CanUpdate() const { return canUpdate; }
		void SetUpdate(bool newCanUpdate) { canUpdate = newCanUpdate; }

		GameObjectType GetGameObjectType() const { return type; }
		void SetGameObjectType(GameObjectType newType) { type = newType; }

		const string& GetName() const { return name; }
		void SetName(const string& newName) { name = newName; }

		u32 GetID() const { return ID; }
		void SetID(u32 newID) { ID = newID; }

		const vec3& GetPos() const { return pos; }
		void SetPos(const vec3& newPos) { pos = newPos; }

		const vec3& GetRot() const { return rot; }
		void SetRot(const vec3& newRot) { rot = newRot; }

		const vec3& GetScale() const { return scale; };
		void SetScale(const vec3& newScale) { scale = newScale; }

		u32 GetVAO() const { return VAO; }
		void SetVAO(u32 newVAO) { VAO = newVAO; }

		u32 GetVBO() const { return VBO; }
		void SetVBO(u32 newVBO) { VBO = newVBO; }

		u32 GetEBO() const { return EBO; }
		void SetEBO(u32 newEBO) { EBO = newEBO; }

		const Shader_OpenGL* GetShader() const { return shader; }
		void SetShader(Shader_OpenGL* newShader) { shader = newShader; }

		virtual bool Render(
			const mat4& view,
			const mat4& projection) = 0;

		virtual ~GameObject() {};
	private:
		bool canUpdate = false;

		GameObjectType type{};

		string name{};
		u32 ID{};
		vec3 pos{};
		vec3 rot{};
		vec3 scale{};

		u32 VAO{};
		u32 VBO{};
		u32 EBO{};

		Shader_OpenGL* shader{};
	};
}