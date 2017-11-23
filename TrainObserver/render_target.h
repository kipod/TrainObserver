#pragma once
#include <d3dx9.h>

enum RenderType
{
	RT_COLOR	= 1,
	RT_Z		= 2
};

class RenderTarget
{
	IDirect3DTexture9*		m_pRenderTarget;
	IDirect3DTexture9*		m_pZTarget;
	IDirect3DSurface9*		m_pDepthStencilTarget;

	IDirect3DSurface9*		m_pLastDepthStencilTarget;
	IDirect3DSurface9*		m_pLastRenderTarget;

	IDirect3DDevice9*		m_pDevice;
	int			m_width;
	int			m_height;
	D3DFORMAT	m_pixelFormat;
	RenderType  m_type;

public:
	RenderTarget( LPDIRECT3DDEVICE9 pD3DDevice, int width, int height, D3DFORMAT pixelFormat );
	~RenderTarget();

	IDirect3DTexture9* pTexture() const { return m_pRenderTarget; }
	IDirect3DTexture9* pZBufferTexture() const { return m_pZTarget; }
	IDirect3DSurface9* pSurface();

	bool push(RenderType type = RT_COLOR);
	void pop();
};
