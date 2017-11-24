#pragma once
#include <vector>
#include <d3d9.h>
#include <string>
#include "render_dx9.h"


class SkyBox: public IRenderable
{
public:
	SkyBox();
	~SkyBox();

	bool create(class RendererDX9& renderer, const char* textures[6]);

	virtual void draw(RendererDX9& renderer) override;

private:
	std::vector<LPDIRECT3DTEXTURE9> m_tiles;
	LPDIRECT3DVERTEXBUFFER9			m_vb = nullptr;

};

