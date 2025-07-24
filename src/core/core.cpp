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
#include "graphics/render.hpp"
#include "graphics/cube.hpp"
#include "graphics/texture.hpp"

//kalacrashhandler
using KalaKit::KalaCrashHandler;

//kalawindow
using KalaWindow::Graphics::ShutdownState;
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::WindowState;
using KalaWindow::Core::Input;
using KalaWindow::Core::Key;
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using GLVState = KalaWindow::Graphics::OpenGL::VSyncState;
using KalaWindow::Graphics::OpenGL::Renderer_OpenGL;

using KalaTestProject::Core::TestProject;
using KalaTestProject::Graphics::Render;
using KalaTestProject::Graphics::Cube;
using KalaTestProject::Graphics::Texture;

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

static inline bool isInitialized = false;
static inline bool isRunning = false;

static inline unsigned int activeSleep{};
static inline unsigned int idleSleep{};

static bool canSleep = true;

static bool isDisplayingTitleData = false;
static void DisplayTitleData();

static void SleepFor(unsigned int ms);

static Window* mainWindow{};

static vec2 lastSize{};

namespace KalaTestProject::Core
{
	void TestProject::Initialize()
	{
		KalaCrashHandler::SetShutdownCallback(Shutdown_Crash);
		KalaCrashHandler::SetProgramName("CircuitGame");

		KalaCrashHandler::Initialize();

		Window::SetUserShutdownFunction(Shutdown);

		string title = "CircuitGame";
		float width = 800;
		float height = 600;

		unique_ptr<Window> newWindow = Window::Initialize(
			title,
			vec2{ width, height });
		mainWindow = newWindow.get();

		if (mainWindow == nullptr) return;

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

		TestProject::Update();
	}

	void TestProject::Update()
	{
		while (isRunning)
		{
			Window::Update(mainWindow);

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

			Input::EndFrameUpdate();

			unsigned int sleepTime = mainWindow->IsIdle() ? idleSleep : activeSleep;
			SleepFor(sleepTime);
		}
	}

	void TestProject::Shutdown()
	{
		Cube::Destroy();
		Texture::createdTextures.clear();
	}

	void TestProject::Shutdown_Crash()
	{
		Window::Shutdown(
			ShutdownState::SHUTDOWN_CRITICAL,
			false);
	}
}

void SleepFor(unsigned int ms)
{
	if (!canSleep) return;

	milliseconds convertedMS = milliseconds(ms);

	time_point<steady_clock> now = steady_clock::now();
	dur frameDuration = now - TestProject::lastFrameTime;

	if (frameDuration < convertedMS) sleep_for(convertedMS - frameDuration);
	TestProject::lastFrameTime = steady_clock::now();
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