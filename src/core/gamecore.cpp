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

static bool isInitialized = false;
static bool isRunning = false;

static unsigned int activeSleep{};
static unsigned int idleSleep{};

static time_point<steady_clock> lastFrameTime{};

static bool canSleep = true;

static bool isDisplayingTitleData = false;
static void DisplayTitleData();

static void SleepFor(unsigned int ms);

static vec2 lastSize{};

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
		KalaCrashHandler::SetProgramName("CircuitGame");

		KalaCrashHandler::Initialize();

		KalaWindowCore::SetUserShutdownFunction(Render::Shutdown);

		string title = "CircuitGame";
		float width = 800;
		float height = 600;
		
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
			<< "4: toggle sleep\n"
			<< "5: toggle fps and resolution in title\n"
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

			if (Input::IsKeyPressed(Key::Num5))
			{
				isDisplayingTitleData = !isDisplayingTitleData;

				if (!isDisplayingTitleData)
				{
					string title = "CircuitGame";
					if (mainWindow->GetTitle() != title)
					{
						mainWindow->SetTitle(title);
					}
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

			if (Input::IsKeyPressed(Key::Num4))
			{
				canSleep = !canSleep;

				string newSleepState = canSleep
					? "Enabled 'canSleep'"
					: "Disabled 'canSleep'";

				Logger::Print(
					newSleepState,
					"TEST_PROJECT",
					LogType::LOG_DEBUG);
			}

			Render::Update();

			createdCamera->UpdateCameraPosition();

			Input::EndFrameUpdate();

			unsigned int sleepTime = mainWindow->IsIdle() ? idleSleep : activeSleep;
			SleepFor(sleepTime);
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

void SleepFor(unsigned int ms)
{
	if (!canSleep) return;

	milliseconds convertedMS = milliseconds(ms);

	time_point<steady_clock> now = steady_clock::now();
	dur frameDuration = now - lastFrameTime;

	if (frameDuration < convertedMS) sleep_for(convertedMS - frameDuration);
	lastFrameTime = steady_clock::now();
}

void DisplayTitleData()
{
	if (!isDisplayingTitleData) return;

	static int frameCount = 0;

	static auto startTime = steady_clock::now();
	static auto lastLogTime = startTime;

	frameCount++;

	auto now = steady_clock::now();
	duration<double> totalElapsed = now - startTime;
	duration<double> logElapsed = now - lastLogTime;

	if (logElapsed.count() >= 0.1)
	{
		double seconds = logElapsed.count();
		double fps = frameCount / seconds;  // use exact duration

		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%.8f", fps);
		char* dot = strchr(buffer, '.');
		if (dot && *(dot + 3)) *(dot + 3) = '\0';

		string fpsStr(buffer);

		/*
		cout << "[+"
			<< static_cast<int>(totalElapsed.count())
			<< "s] Frames: " << fpsStr << "\n";
		*/

		vec2 winSize = mainWindow->GetSize();
		if (lastSize.x != winSize.x
			|| lastSize.y != winSize.y)
		{
			lastSize = winSize;
		}
		string resolution =
			to_string(static_cast<int>(lastSize.x)) + "x" +
			to_string(static_cast<int>(lastSize.y));

		string title = "CircuitGame [ " + resolution + " ] [ " + fpsStr + " fps ]";
		mainWindow->SetTitle(title);

		frameCount = 0;
		lastLogTime = now;
	}
}