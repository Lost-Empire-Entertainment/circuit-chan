//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

//kalawindow
#include "graphics/window.hpp"

#include "gameobjects/cube.hpp"
#include "graphics/camera.hpp"

namespace CircuitGame::Core
{
	using KalaWindow::Graphics::Window;

	using CircuitGame::Graphics::Camera;
	using CircuitGame::GameObjects::Cube;

	using std::unordered_map;
	using std::vector;
	using std::unique_ptr;

	//
	// INIT STAGE DATA
	//

	extern unordered_map<u32, unique_ptr<Cube>> createdCubes;
	extern unique_ptr<Camera> createdCamera;
	extern Window* mainWindow;

	//
	// RUNTIME STAGE DATA
	//

	extern vector<Cube*> runtimeCubes;

	class Game
	{
	public:
		//Initializes all parts of this program
		static void Initialize();

		//The core program uodate loop
		static void Update();

		//Fully shuts down this program, used when a crash condition is detected
		static void Shutdown_Crash();
	};
}