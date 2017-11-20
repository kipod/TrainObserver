#pragma once
#include <unordered_map>
#include <memory>
#include <d3d9.h>

typedef IDirect3DTexture9 Texture;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	Texture* get(const std::string& path);

private:
	std::unordered_map<std::string, Texture* > m_textures;
};

