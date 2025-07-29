//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

#include "gameobjects/cube.hpp"
#include "graphics/camera.hpp"

namespace CircuitGame::Graphics
{
	using std::unordered_map;
	using std::vector;
	using std::unique_ptr;
	using std::string;

	using CircuitGame::GameObjects::Cube;
	using CircuitGame::Graphics::Camera;

	class Render
	{
	public:
		static inline unordered_map<u32, unique_ptr<Cube>> createdCubes{};
		static inline unique_ptr<Camera> createdCamera{};
		static inline Window* mainWindow{};

		static inline vector<Cube*> runtimeCubes{};

		//Initializes the render loop
		static bool Initialize();

		//The full graphical render loop of this program
		static void Update();

		//What to call when we need to redraw during rescaling the window etc
		static void Redraw();

		//Destroy all created textures and gameobjects
		static void Shutdown();
	};
}