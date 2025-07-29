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

	using std::string;

	using KalaWindow::Graphics::OpenGL::Shader_OpenGL;

	enum class GameObjectType
	{
		cube,
		pointLight,
		dirLight
	};

	class GameObject
	{
	public:
		virtual GameObject* Initialize(
			const string& name,
			Shader_OpenGL* shader,
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0),
			const vec3& scale = vec3(1)) = 0;

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

		const Shader_OpenGL* GetShader() const { return shader; }
		void SetShader(Shader_OpenGL* newShader) { shader = newShader; }

		virtual bool Render() = 0;

		virtual ~GameObject() {};
	private:
		bool canUpdate = false;

		GameObjectType type{};

		string name{};
		u32 ID{};
		vec3 pos{};
		vec3 rot{};
		vec3 scale{};

		Shader_OpenGL* shader{};
	};
}