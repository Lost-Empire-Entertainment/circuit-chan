//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

namespace CircuitGame::Graphics
{
	using std::string;

	class Texture
	{
	public:
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