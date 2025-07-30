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
		if (createdCamera != nullptr)
		{
			auto GetNewPos = [](Action action, Direction dir) -> vec3
				{
					vec3 pos = createdCamera->GetPos();

					float speed = createdCamera->GetSpeed();
					f32 deltaTime = static_cast<f32>(Game::GetDeltaTime());

					float addition = speed * deltaTime;

					switch (dir)
					{
					case Direction::DIR_X:
						pos.x = action == Action::ACTION_SUBTRACT
							? pos.x - addition
							: pos.x + addition;

						return pos;
					case Direction::DIR_Y:
						pos.y = action == Action::ACTION_SUBTRACT
							? pos.y - addition
							: pos.y + addition;

						return pos;
					case Direction::DIR_Z:
						pos.z = action == Action::ACTION_SUBTRACT
							? pos.z - addition
							: pos.z + addition;

						return pos;
					}

					return vec3(0);
				};

			if (Input::IsKeyDown(Key::Q))
			{
				createdCamera->SetPos(GetNewPos(Action::ACTION_SUBTRACT, Direction::DIR_Y));
			}
			if (Input::IsKeyDown(Key::E))
			{
				createdCamera->SetPos(GetNewPos(Action::ACTION_ADD, Direction::DIR_Y));
			}

			if (Input::IsKeyDown(Key::W))
			{
				createdCamera->SetPos(GetNewPos(Action::ACTION_SUBTRACT, Direction::DIR_Z));
			}
			if (Input::IsKeyDown(Key::S))
			{
				createdCamera->SetPos(GetNewPos(Action::ACTION_ADD, Direction::DIR_Z));
			}
			if (Input::IsKeyDown(Key::A))
			{
				createdCamera->SetPos(GetNewPos(Action::ACTION_SUBTRACT, Direction::DIR_X));
			}
			if (Input::IsKeyDown(Key::D))
			{
				createdCamera->SetPos(GetNewPos(Action::ACTION_ADD, Direction::DIR_X));
			}
		}
	}
}