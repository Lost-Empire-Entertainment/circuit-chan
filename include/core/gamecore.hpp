//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

namespace CircuitGame::Core
{
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