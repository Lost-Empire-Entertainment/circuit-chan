//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <string>

//kalawindow
#include "core/input.hpp"
#include "core/log.hpp"

#include "core/playerinput.hpp"
#include "graphics/camera.hpp"
#include "core/gamecore.hpp"

using KalaWindow::Core::Input;
using KalaWindow::Core::Key;
using KalaWindow::Core::MouseButton;
using KalaWindow::Core::Logger;

using CircuitGame::Core::Game;
using CircuitGame::Graphics::Camera;
using CircuitGame::Core::createdCamera;

using std::to_string;
using std::string;
using glm::radians;

enum class Action
{
	ACTION_ADD,
	ACTION_SUBTRACT
};
enum class Direction
{
	DIR_X,
	DIR_Y,
	DIR_Z
};

namespace CircuitGame::Core
{
	void PlayerInput::HandleInput()
	{
		if (mainWindow == nullptr
			|| !mainWindow->IsFocused()
			|| createdCamera == nullptr)
		{
			return;
		}

		if (Input::IsKeyPressed(Key::Escape))
		{
			bool currentMoveState = createdCamera->CanMove();

			currentMoveState = !currentMoveState;

			string state = currentMoveState ? "true" : "false";
			Logger::Print("Set camera move state to '" + state + "'!");

			createdCamera->SetMoveState(currentMoveState);
		}

		if (!createdCamera->CanMove())
		{
			if (Input::GetKeepMouseDeltaState())
			{
				Input::SetMouseLockState(false);
				Input::SetMouseVisibility(true);
				Input::SetKeepMouseDeltaState(false);
			}
			return;
		}

		if (!Input::GetKeepMouseDeltaState())
		{
			Input::SetMouseLockState(true);
			Input::SetMouseVisibility(false);
			Input::SetKeepMouseDeltaState(true);
		}

		vec2 mouseDelta = Input::GetRawMouseDelta();
		createdCamera->UpdateCameraRotation(mouseDelta);

		const vec3& front = createdCamera->GetFront();
		const vec3& right = createdCamera->GetRight();
		const vec3& up = createdCamera->GetUp();

		float velocity = createdCamera->GetSpeed() * static_cast<f32>(Game::GetDeltaTime());
		vec3 pos = createdCamera->GetPos();

		if (Input::IsKeyDown(Key::W)) pos += front * velocity;
		if (Input::IsKeyDown(Key::S)) pos -= front * velocity;
		if (Input::IsKeyDown(Key::A)) pos -= right * velocity;
		if (Input::IsKeyDown(Key::D)) pos += right * velocity;

		//TODO: INVESTIGATE WHY LEFT MOUSE BUTTON ALSO MOVES CAMERA UP

		//Q/E always goes up and down relative to world Y
		if (Input::IsKeyDown(Key::Q)) pos -= up * velocity;
		if (Input::IsKeyDown(Key::E)) pos += up * velocity;

		createdCamera->SetPos(pos);
	}
}