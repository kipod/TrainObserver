#include "texture_manager.h"
#include "render_dx9.h"



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	for (auto& tex : m_textures)
	{
		tex.second->Release();
	}
}

Texture* TextureManager::get(const std::string& path)
{
	auto found = m_textures.find(path);

	if (found != m_textures.end())
	{
		return found->second;
	}

	
	Texture* newTexture = nullptr;
	auto hr = D3DXCreateTextureFromFile(RenderSystemDX9::instance().renderer().device(), path.c_str(), &newTexture);

	if (newTexture)
	{
		m_textures.insert(std::make_pair(path, newTexture));
	}

	return newTexture;
}
