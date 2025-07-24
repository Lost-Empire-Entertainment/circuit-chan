//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <chrono>

namespace KalaTestProject::Core
{
	using std::chrono::time_point;
	using std::chrono::steady_clock;

	class TestProject
	{
	public:
		static inline time_point<steady_clock> lastFrameTime{};

		//Initializes all parts of this program
		static void Initialize();

		//The core program uodate loop
		static void Update();

		//Standard shutdown conditions, called when user wants to shut down program regularly
		static void Shutdown();

		//Fully shuts down this program, used when a crash condition is detected
		static void Shutdown_Crash();
	};
}