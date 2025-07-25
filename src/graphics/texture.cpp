//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <filesystem>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"

//kalawindow
#include "core/log.hpp"
#include "graphics/opengl/opengl_core.hpp"
#include "graphics/window.hpp"

#include "graphics/texture.hpp"
#include "graphics/render.hpp"

//kalawindow
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;
using KalaWindow::Core::TimeFormat;
using KalaWindow::Core::DateFormat;
using KalaWindow::Graphics::Window;
using KalaWindow::Graphics::PopupAction;
using KalaWindow::Graphics::PopupResult;
using KalaWindow::Graphics::PopupType;
using KalaWindow::Graphics::ShutdownState;

using CircuitGame::Graphics::Texture;
using CircuitGame::Graphics::Render;

using std::make_unique;
using std::move;
using std::find;
using std::filesystem::exists;
using std::filesystem::path;
using std::filesystem::current_path;
using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::vector;

enum class TextureCheckResult
{
	RESULT_OK,
	RESULT_INVALID,
	RESULT_ALREADY_EXISTS
};

static void ForceClose(
	const string& title,
	const string& reason);

static TextureCheckResult IsValidTexture(
	const string& textureName,
	const string& texturePath,
	const unordered_map<string, unique_ptr<Texture>>& textures);

namespace CircuitGame::Graphics
{
	Texture* Texture::CreateTexture(
		const string& textureName,
		const string& texturePath)
	{
		TextureCheckResult result = (IsValidTexture(
			textureName,
			texturePath,
			Render::createdTextures));

		if (result == TextureCheckResult::RESULT_INVALID) return nullptr;
		else if (result == TextureCheckResult::RESULT_ALREADY_EXISTS)
		{
			return Render::createdTextures[textureName].get();
		}

		Logger::Print(
			"Loading texture '" + textureName + "'.",
			"TEXTURE",
			LogType::LOG_INFO);

		unsigned int newTextureID{};
		glGenTextures(1, &newTextureID);
		glBindTexture(GL_TEXTURE_2D, newTextureID);

		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			GL_REPEAT);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			GL_REPEAT);

		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);

		int width{};
		int height{};
		int nrChannels{};
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(
			(texturePath).c_str(),
			&width,
			&height,
			&nrChannels,
			0);

		if (!data)
		{
			Logger::Print(
				"Failed to load texture '" + texturePath + "'!",
				"TEXTURE",
				LogType::LOG_ERROR,
				2);

			return nullptr;
		}

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data);

		unique_ptr<Texture> tex = make_unique<Texture>();
		tex->textureID = newTextureID;
		tex->texturePath = texturePath;
		Render::createdTextures[textureName] = move(tex);

		Logger::Print(
			"Loaded texture '" + texturePath + "'!",
			"TEXTURE",
			LogType::LOG_SUCCESS);

		return Render::createdTextures[textureName].get();
	}

	Texture::~Texture()
	{
		
	}
}

void ForceClose(
	const string& title,
	const string& reason)
{
	Logger::Print(
		reason,
		"TEXTURE",
		LogType::LOG_ERROR,
		2,
		TimeFormat::TIME_NONE,
		DateFormat::DATE_NONE);

	Window* mainWindow = Window::windows.front();
	if (mainWindow->CreatePopup(
		title,
		reason,
		PopupAction::POPUP_ACTION_OK,
		PopupType::POPUP_TYPE_ERROR)
		== PopupResult::POPUP_RESULT_OK)
	{
		Window::Shutdown(ShutdownState::SHUTDOWN_FAILURE);
	}
}

TextureCheckResult IsValidTexture(
	const string& textureName,
	const string& texturePath,
	const unordered_map<string, unique_ptr<Texture>>& createdTextures)
{
	//texture name must not be empty

	if (textureName.empty())
	{
		string title = "Texture error [texture]";
		string reason = "Cannot load a texture with no name!";

		ForceClose(title, reason);
	}

	//texture path must not be empty

	if (texturePath.empty())
	{
		string title = "Texture error [texture]";
		string reason = "Cannot load a texture with no path!";

		ForceClose(title, reason);
	}

	string texturePathName = path(texturePath).filename().string();

	//texture file must exist

	if (!exists(texturePath))
	{
		string title = "Texture error [texture]";
		string reason = "Texture '" + textureName + "' path '" + texturePathName + "' does not exist!";

		ForceClose(title, reason);

		return TextureCheckResult::RESULT_INVALID;
	}

	vector<string> validExtensions =
	{
		".png",
		".jpg",
		".jpeg"
	};

	//texture file must have an extension

	if (!path(texturePath).has_extension())
	{
		string title = "Texture error [texture]";
		string reason = "Texture '" + textureName + "' has no extension. You must use png, jpg or jpeg!";

		ForceClose(title, reason);

		return TextureCheckResult::RESULT_INVALID;
	}

	string thisExtension = path(texturePath).extension().string();
	bool isExtensionValid = 
		find(validExtensions.begin(),
			validExtensions.end(),
			thisExtension)
		!= validExtensions.end();

	//texture file must have a valid extension

	if (!isExtensionValid)
	{
		string title = "Texture error [texture]";
		string reason = "Texture '" + textureName + "' has an invalid extension '" + thisExtension + "'. Only png, jpg and jpeg are allowed!";

		ForceClose(title, reason);

		return TextureCheckResult::RESULT_INVALID;
	}

	//pass existing texture if one with the same name or path already exists

	for (const auto& [key, value] : createdTextures)
	{
		if (key == textureName)
		{
			Logger::Print(
				"Texture '" + textureName + "' already exists!",
				"TEXTURE",
				LogType::LOG_ERROR,
				2);

			return TextureCheckResult::RESULT_ALREADY_EXISTS;
		}

		if (value->GetTexturePath() == texturePath)
		{
			Logger::Print(
				"Texture '" + textureName + "' with path '" + texturePathName + "' has already been loaded!",
				"TEXTURE",
				LogType::LOG_ERROR,
				2);

			return TextureCheckResult::RESULT_ALREADY_EXISTS;
		}
	}

	return TextureCheckResult::RESULT_OK;
}