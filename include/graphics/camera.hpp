//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <algorithm>

//kalawindow
#include "core/platform.hpp"
#include "graphics/window.hpp"

namespace CircuitGame::Graphics
{
	using KalaWindow::Graphics::Window;

	using std::clamp;

	class Camera
	{
	public:
		static Camera* CreateCamera(
			Window* window,
			f32 fov = 90.0f,
			f32 nearClip = 0.01f,
			f32 farClip = 500.0f,
			f32 speed = 25.0f,
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0));

		bool CanMove() const { return canMove; };
		void SetMoveState(bool newMove) { canMove = newMove; }

		//Handle camera rotation based off of mouse movement
		void UpdateCameraRotation(vec2 delta);

		f32 GetFOV() const { return fov; }
		void SetFOV(f32 newFOV)
		{
			fov = clamp(newFOV, 70.0f, 110.0f);
		}

		f32 GetNearClip() const { return nearClip; }
		void SetNearClip(f32 newNearClip)
		{
			nearClip = clamp(newNearClip, 0.001f, farClip - 0.1f);
		}

		f32 GetFarClip() const { return farClip; }
		void SetFarClip(f32 newFarClip)
		{
			farClip = clamp(newFarClip, nearClip + 0.1f, 1000.0f);
		}

		f32 GetAspectRatio() const { return aspectRatio; }
		//Called inside resize callback to ensure camera aspect ratio always stays valid
		void SetAspectRatio(f32 size)
		{
			aspectRatio = clamp(size, 0.001f, 10.0f);
		}

		f32 GetSpeed() const { return speed; }
		void SetSpeed(f32 newSpeed)
		{
			speed = clamp(newSpeed, 0.1f, 10.0f);
		}

		f32 GetSensitivity() const { return sensitivity; }
		void SetSensitivity(f32 newSens)
		{
			sensitivity = clamp(newSens, 0.001f, 10.0f);
		}

		mat4 GetViewMatrix() const;

		const vec3& GetUp() const { return up; }
		void SetFront(const vec3& newFront) { front = newFront; }
		const vec3& GetFront() const { return front; }
		void SetRight(const vec3& newRight) { right = newRight; }
		const vec3& GetRight() const { return right; }

		const vec3& GetPos() const { return pos; }
		void SetPos(const vec3& newPos)
		{
			pos =
			{
				clamp(newPos.x, -10000.0f, 10000.0f),
				clamp(newPos.y, -10000.0f, 10000.0f),
				clamp(newPos.z, -10000.0f, 10000.0f)
			};
		}

		const vec3& GetRot() const { return rot; }
		//Safely clamps within allowed bounds
		void SetRot(const vec3& newRot)
		{
			auto CorrectRot = [](f32 angle)
				{
					angle = fmodf(angle, 360.0f);
					if (angle < 0.0f) angle += 360.0f;
					return angle;
				};

			rot =
			{
				clamp(newRot.x, 0.0f, 359.99f),
				clamp(newRot.y, 0.0f, 359.99f),
				clamp(newRot.z, 0.0f, 359.99f),
			};
		}
		//Safely wraps within allowed bounds
		void AddRot(const vec3& deltaRot)
		{
			auto WrapAngle = [](f32 angle)
				{
					angle = fmodf(angle, 360.0f);
					if (angle < 0.0f) angle += 360.0f;
					return angle;
				};

			rot =
			{
				WrapAngle(rot.x + deltaRot.x),
				WrapAngle(rot.y + deltaRot.y),
				WrapAngle(rot.z + deltaRot.z),
			};
		}

		~Camera();
	private:
		bool canMove = false;

		f32 fov = 90.0f;
		f32 nearClip = 0.01f;
		f32 farClip = 500.0f;
		f32 aspectRatio = 1.777777f; //For 16/9 aspect ratio
		f32 speed = 1.0f;
		f32 sensitivity = 0.1f;

		vec3 up = vec3(0.0f, 1.0f, 0.0f);
		vec3 front = vec3(0.0f, 0.0f, -1.0f);
		vec3 right = vec3(1.0f, 0.0f, 0.0f);

		vec3 pos = vec3(0);
		vec3 rot = vec3(0);
	};
}