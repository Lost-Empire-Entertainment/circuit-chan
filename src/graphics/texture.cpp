//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <filesystem>
#include <string>
#include <unordered_map>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"

//kalawindow
#include "core/log.hpp"
#include "graphics/opengl/opengl_core.hpp"

#include "graphics/texture.hpp"
#include "graphics/render.hpp"

//kalawindow
using KalaWindow::Core::Logger;
using KalaWindow::Core::LogType;

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

static bool IsValidTexture(
	const string& textureName,
	const string& texturePath,
	const unordered_map<string, unique_ptr<Texture>>& textures);

namespace CircuitGame::Graphics
{
	Texture* Texture::CreateTexture(
		const string& textureName,
		const string& texturePath)
	{
		if (!IsValidTexture(
			textureName,
			texturePath,
			Render::createdTextures))
		{
			return nullptr;
		}

		Logger::Print(
			"Creating texture '" + textureName + "'.",
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
				"Failed to create texture '" + texturePath + "'!",
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
			"Created texture '" + texturePath + "'!",
			"TEXTURE",
			LogType::LOG_SUCCESS);

		return Render::createdTextures[textureName].get();
	}

	Texture::~Texture()
	{
		
	}
}

bool IsValidTexture(
	const string& textureName,
	const string& texturePath,
	const unordered_map<string, unique_ptr<Texture>>& createdTextures)
{
	if (!exists(texturePath))
	{
		Logger::Print(
			"Texture path '" + texturePath + "' is invalid!",
			"TEXTURE",
			LogType::LOG_ERROR,
			2);

		return false;
	}

	for (const auto& [key, value] : createdTextures)
	{
		if (key == textureName)
		{
			Logger::Print(
				"Texture with name '" + textureName + "' already exists!",
				"TEXTURE",
				LogType::LOG_ERROR,
				2);

			return false;
		}

		if (value->GetTexturePath() == texturePath)
		{
			Logger::Print(
				"Texture with path '" + texturePath + "' has already been loaded!",
				"TEXTURE",
				LogType::LOG_ERROR,
				2);

			return false;
		}
	}

	return true;
}