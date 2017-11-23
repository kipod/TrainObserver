#pragma once
#include <memory>
#include <d3d9.h>
#include "effect.h"

class Supersampler
{
public:
	Supersampler(LPDIRECT3DDEVICE9 pD3DDevice, int width, int height, D3DFORMAT pixelFormat);
	~Supersampler();

	void push();
	void pop(LPDIRECT3DDEVICE9 pDevice);

private:
	void draw(LPDIRECT3DDEVICE9 pDevice);

private:
	std::unique_ptr<class RenderTarget> m_backBuffer;
	std::unique_ptr<class Quad>			m_quad;
	Effect*								m_effect;
	EffectProperties					m_props;
};

