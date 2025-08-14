//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>

//kalacrashhandler
#include "crashHandler.hpp"

//kalawindow
#include "graphics/opengl/opengl.hpp"
#include "graphics/window.hpp"
#include "core/input.hpp"
#include "core/log.hpp"
#include "core/core.hpp"
#include "core/containers.hpp"

#include "core/gamecore.hpp"
#include "core/playerinput.hpp"
#include "graphics/render.hpp"

//kalacrashhandler
using KalaKit::KalaCrashHandler;

//kalawindow
using KalaWindow::Core::KalaWindowCore;
using KalaWindow::Core::ShutdownState;
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::WindowState;
using KalaWindow::Core::Input;
using KalaWindow::Core::Key;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using GLVState = KalaWindow::Graphics::OpenGL::VSyncState;
using KalaWindow::Graphics::OpenGL::Renderer_OpenGL;
using KalaWindow::Core::createdWindows;
using KalaWindow::Core::runtimeWindows;

using CircuitGame::Core::Game;
using CircuitGame::Graphics::Render;
using CircuitGame::Core::mainWindow;

using std::thread;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
using std::chrono::steady_clock;
using dur = std::chrono::steady_clock::duration;
using std::chrono::time_point;
using std::chrono::duration;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::to_string;
using std::stringstream;
using std::clamp;

static string title = "Circuit Chan 0.0.3 Alpha";

static bool isInitialized = false;
static bool isRunning = false;

static bool isDisplayingTitleData = false;
static void DisplayTitleData();

static void UpdateDeltaTime();

static vec2 lastSize{};

static f64 accumulator = 0.0;

namespace CircuitGame::Core
{
	//
	// DEFINE INIT AND RUNTIME STAGE DATA
	//

	unordered_map<u32, unique_ptr<Cube>> createdCubes{};
	unique_ptr<Camera> createdCamera{};
	Window* mainWindow{};
	vector<Cube*> runtimeCubes{};

	void Game::Initialize()
	{
		KalaCrashHandler::SetShutdownCallback(Shutdown_Crash);
		KalaCrashHandler::SetProgramName("Circuit Chan");

		KalaCrashHandler::Initialize();

		KalaWindowCore::SetUserShutdownFunction(Render::Shutdown);

		f32 width = 800;
		f32 height = 600;
		
		mainWindow = Window::Initialize(
			title,
			vec2{ width, height });

		if (mainWindow == nullptr)
		{
			KalaWindowCore::ForceClose(
				"Window error",
				"Failed to create main window!");
		}

		if (!Input::Initialize(mainWindow)) return;

		Logger::Print(
			"Initializing OpenGL...",
			"TEST_PROJECT",
			LogType::LOG_INFO);

		if (!Render::Initialize()) return;
		Renderer_OpenGL::SetVSyncState(GLVState::VSYNC_ON);

		mainWindow->SetMinSize(vec2{ 800, 600 });
		mainWindow->SetMaxSize(vec2{ 3840, 2160 });

		stringstream ss{};
		ss << "\n====================\n"
			<< "1: set vsync on\n"
			<< "2: set vsync off\n"
			<< "3: set vsync to triple buffering (vulkan only)\n"
			<< "4: toggle fps and resolution in title\n"
			<< "====================";

		Logger::Print(
			ss.str(),
			"TEST_PROJECT",
			LogType::LOG_INFO);

		isInitialized = true;
		isRunning = true;

		mainWindow->SetWindowState(WindowState::WINDOW_MAXIMIZE);

		Game::Update();
	}

	void Game::Update()
	{
		while (isRunning)
		{
			UpdateDeltaTime();
			PlayerInput::HandleInput();

			/*
			* 
			* //Physics should run on its own thread to not slow down the render thread!
			* 
			* i32 maxSteps = 5;
			* i32 steps = 0;
			* 
			while (accumulator >= GetFixedDelta()
				   && steps < maxSteps)
			{
				//<<<< physics logic here
				accumulator -= GetFixedDelta();
				steps++;
			}
			*/

			mainWindow->Update();

			if (Input::IsKeyPressed(Key::Num1))
			{
				Renderer_OpenGL::SetVSyncState(GLVState::VSYNC_ON);
				
				Logger::Print(
					"Set 'vsync state' to 'ON'",
					"CORE",
					LogType::LOG_DEBUG);
			}
			if (Input::IsKeyPressed(Key::Num2))
			{
				Renderer_OpenGL::SetVSyncState(GLVState::VSYNC_OFF);

				Logger::Print(
					"Set 'vsync state' to 'OFF'",
					"CORE",
					LogType::LOG_DEBUG);
			}
			if (Input::IsKeyPressed(Key::Num3))
			{
				Logger::Print(
					"Cannot set 'vsync state' to 'TRIPLE BUFFERING' because OpenGL does not have it!",
					"CORE",
					LogType::LOG_ERROR,
					2);
			}

			if (Input::IsKeyPressed(Key::Num4))
			{
				isDisplayingTitleData = !isDisplayingTitleData;

				if (!isDisplayingTitleData
					&& mainWindow->GetTitle() != title)
				{
					mainWindow->SetTitle(title);
				}

				string newDisplayTitleData = isDisplayingTitleData
					? "Enabled 'display title data'"
					: "Disabled 'display title data'";

				Logger::Print(
					newDisplayTitleData,
					"TEST_PROJECT",
					LogType::LOG_DEBUG);
			}
			DisplayTitleData();

			Render::Update();

			Input::EndFrameUpdate();
		}
	}

	void Game::Shutdown_Crash()
	{
		Render::Shutdown();

		KalaWindowCore::Shutdown(
			ShutdownState::SHUTDOWN_CRITICAL,
			false);
	}
}

void UpdateDeltaTime()
{
	auto now = steady_clock::now();
	static time_point<steady_clock> lastFrameTime = now;

	duration<f64> delta = now - lastFrameTime;
	lastFrameTime = now;

	//unscaled, unclamped
	f64 frameTime = delta.count();

	//regular deltatime
	f64 deltaTime = clamp(delta.count(), 0.0, 0.1);

	Game::SetFrameTime(frameTime);
	Game::SetDeltaTime(deltaTime);
}

void DisplayTitleData()
{
	if (!isDisplayingTitleData) return;

	static auto startTime = steady_clock::now();
	static auto lastLogTime = startTime;

	static f64 accumFrameTime = 0.0;
	static unsigned int frameCount = 0;
	static f64 fps = 0.0;
	static f64 deltaTime = 0.0;

	auto now = steady_clock::now();
	duration<f64> totalElapsed = now - startTime;
	duration<f64> logElapsed = now - lastLogTime;

	accumFrameTime += Game::GetFrameTime();
	++frameCount;

	if (accumFrameTime > 0.0
		&& frameCount > 0)
	{
		fps = static_cast<f64>(frameCount) / accumFrameTime;
		deltaTime = (accumFrameTime * 1000.0) / static_cast<f64>(frameCount);
	}

	if (logElapsed.count() >= 0.1)
	{
		//window size

		vec2 winSize = mainWindow->GetSize();
		if (lastSize.x != winSize.x
			|| lastSize.y != winSize.y)
		{
			lastSize = winSize;
		}
		string resolution =
			to_string(static_cast<int>(lastSize.x)) + "x" +
			to_string(static_cast<int>(lastSize.y));

		//fps

		char fpsBuffer[32];
		snprintf(fpsBuffer, sizeof(fpsBuffer), "%.8f", fps);
		char* fpsDot = strchr(fpsBuffer, '.');
		if (fpsDot && *(fpsDot + 3)) *(fpsDot + 3) = '\0';

		string fpsStr(fpsBuffer);

		//deltatime

		char dtBuffer[32];
		snprintf(dtBuffer, sizeof(dtBuffer), "%.8f", deltaTime);
		char* dtDot = strchr(dtBuffer, '.');
		if (dtDot && *(dtDot + 3)) *(dtDot + 3) = '\0';

		string dtStr(dtBuffer);

		//display data

		string fullTitle = title + " [ " + resolution + " ] [ " + fpsStr + " FPS (" + dtStr + "ms) ]";
		mainWindow->SetTitle(fullTitle);

		lastLogTime = now;
		accumFrameTime = 0.0;
		frameCount = 0;
	}
}