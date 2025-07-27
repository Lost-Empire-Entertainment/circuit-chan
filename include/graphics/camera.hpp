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
			float fov,
			float nearClip = 0.01f,
			float farClip = 500.0f,
			float aspectRatio = 1.777777f, //default 16/9 aspect ratio
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0)) {}; //TODO: DEFINE

		float GetFOV() const { return fov; }
		void SetFOV(float newFOV)
		{
			fov = clamp(newFOV, 70.0f, 110.0f);
		}

		float GetNearClip() const { return nearClip; }
		void SetNearClip(float newNearClip)
		{
			nearClip = clamp(newNearClip, 0.001f, farClip - 0.1f);
		}

		float GetFarClip() const { return farClip; }
		void SetFarClip(float newFarClip)
		{
			farClip = clamp(newFarClip, nearClip + 0.1f, 1000.0f);
		}

		float GetAspectRatio() const { return aspectRatio; }
		//Should be called inside redrawCallback, that way aspect ratio is only updated if window size actually changes
		//TODO: ADD ASPECT RATIO UPDATE TO RESIZE CALLBACK
		void SetAspectRatio(Window* window) {}; //TODO: DEFINE

		const vec3& GetUp() const { return up; }
		const vec3& GetFront() const { return front; }

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
			auto CorrectRot = [](float angle)
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
			auto WrapAngle = [](float angle)
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

		~Camera() {}; //TODO: DEFINE
	private:
		float fov = 90.0f;
		float nearClip = 0.01f;
		float farClip = 500.0f;
		float aspectRatio = 1.777777f; //For 16/9 aspect ratio

		vec3 up = vec3(0, 1, 0);
		vec3 front = vec3(1, 0, 0);

		vec3 pos = vec3(0);
		vec3 rot = vec3(0);
	};
}