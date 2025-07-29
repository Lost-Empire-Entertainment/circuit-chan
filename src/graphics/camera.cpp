//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <memory>

#include "glm/gtc/matrix_transform.hpp"

//kalawindow
#include "core/log.hpp"
#include "core/input.hpp"

#include "graphics/camera.hpp"
#include "core/gamecore.hpp"

//kalawindow
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Core::Input;
using KalaWindow::Core::Key;

using CircuitGame::Core::createdCamera;

using std::unique_ptr;
using std::make_unique;
using glm::lookAt;

namespace CircuitGame::Graphics
{
	Camera* Camera::CreateCamera(
		Window* window,
		float fov,
		float nearClip,
		float farClip,
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
		float aspectRatio = size.x / size.y;
		cameraPtr->SetAspectRatio(aspectRatio);

		cameraPtr->SetPos(pos);
		cameraPtr->SetRot(rot);

		Logger::Print(
			"Created camera!",
			"CAMERA",
			LogType::LOG_SUCCESS);

		return cameraPtr;
	}

	void Camera::UpdateCameraPosition()
	{
		if (Input::IsKeyDown(Key::Q))
		{
			pos.y -= 0.005f;
		}
		if (Input::IsKeyDown(Key::E))
		{
			pos.y += 0.005f;
		}

		if (Input::IsKeyDown(Key::W))
		{
			pos.z -= 0.005f;
		}
		if (Input::IsKeyDown(Key::S))
		{
			pos.z += 0.005f;
		}
		if (Input::IsKeyDown(Key::A))
		{
			pos.x -= 0.005f;
		}
		if (Input::IsKeyDown(Key::D))
		{
			pos.x += 0.005f;
		}
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