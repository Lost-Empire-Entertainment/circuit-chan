//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <unordered_map>
#include <memory>
#include <string>

namespace KalaTestProject::Graphics
{
	using std::unordered_map;
	using std::unique_ptr;
	using std::string;

	class Texture
	{
	public:
		static inline unordered_map<string, unique_ptr<Texture>> createdTextures{};

		static Texture* CreateTexture(
			const string& textureName,
			const string& texturePath);

		unsigned int GetTextureID() const { return textureID; }
		const string& GetTexturePath() const { return texturePath; }

		~Texture();
	private:
		unsigned int textureID{};
		string texturePath{};
	};
}