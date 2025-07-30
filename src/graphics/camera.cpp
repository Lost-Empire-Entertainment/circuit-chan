//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <memory>

#include "glm/gtc/matrix_transform.hpp"

//kalawindow
#include "core/log.hpp"

#include "graphics/camera.hpp"
#include "core/gamecore.hpp"

//kalawindow
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;

using CircuitGame::Core::createdCamera;

using std::unique_ptr;
using std::make_unique;
using glm::lookAt;

namespace CircuitGame::Graphics
{
	Camera* Camera::CreateCamera(
		Window* window,
		f32 fov,
		f32 nearClip,
		f32 farClip,
		f32 speed,
		const vec3& pos,
		const vec3& rot)
	{
		if (createdCamera != nullptr)
		{
			Logger::Print(
				"Cannot create a new camera because a camera already exists!",
				"CAMERA",
				LogType::LOG_ERROR,
				2);

			return nullptr;
		}

		Logger::Print(
			"Creating camera.",
			"CAMERA",
			LogType::LOG_DEBUG);

		unique_ptr<Camera> camera = make_unique<Camera>();
		createdCamera = move(camera);
		Camera* cameraPtr = createdCamera.get();

		cameraPtr->SetFOV(fov);
		cameraPtr->SetNearClip(nearClip);
		cameraPtr->SetFarClip(farClip);

		vec2 size = window->GetSize();
		f32 aspectRatio = size.x / size.y;
		cameraPtr->SetAspectRatio(aspectRatio);

		cameraPtr->SetPos(pos);
		cameraPtr->SetRot(rot);

		Logger::Print(
			"Created camera!",
			"CAMERA",
			LogType::LOG_SUCCESS);

		return cameraPtr;
	}

	void Camera::UpdateCameraRotation(vec2 delta)
	{

	}

	mat4 Camera::GetViewMatrix() const
	{
		return lookAt(
			pos,
			pos + front,
			up);
	}

	Camera::~Camera()
	{
		Logger::Print(
			"Destroyed camera!",
			"CAMERA",
			LogType::LOG_SUCCESS);
	}
}