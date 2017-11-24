#include "render_target.h"

RenderTarget::RenderTarget( LPDIRECT3DDEVICE9 pD3DDevice, int width, int height, D3DFORMAT pixelFormat ):
m_pixelFormat(pixelFormat),
m_width(width),
m_height(height),
m_pRenderTarget(NULL),
m_pZTarget(NULL),
m_pDepthStencilTarget(NULL),
m_pDevice(pD3DDevice)
{
	HRESULT hr;
	hr = pD3DDevice->CreateTexture(
		width, height, 1, D3DUSAGE_RENDERTARGET, pixelFormat, D3DPOOL_DEFAULT, &m_pRenderTarget, NULL );

	hr = pD3DDevice->CreateTexture(
		width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pZTarget, NULL );

	hr = pD3DDevice->CreateDepthStencilSurface(
		width, height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0,
		true, &m_pDepthStencilTarget, NULL );
}


RenderTarget::~RenderTarget(void)
{
	if(m_pRenderTarget)
		m_pRenderTarget->Release();
	if(m_pDepthStencilTarget)
		m_pDepthStencilTarget->Release();
}

bool RenderTarget::push(RenderType type)
{
	if(!m_pRenderTarget)
		return false;

	m_type = type;

	HRESULT hr;

	if(type & RT_COLOR)
	{
		hr = m_pDevice->GetRenderTarget(0, &m_pLastRenderTarget );
		hr |= m_pDevice->GetDepthStencilSurface( &m_pLastDepthStencilTarget );
		if ( FAILED( hr ) )
			return false;
		m_pLastRenderTarget->Release();
		m_pLastDepthStencilTarget->Release();

		IDirect3DSurface9* pSurface = NULL;
		hr = m_pRenderTarget->GetSurfaceLevel( 0, &pSurface );

		if ( FAILED( hr ) )
			return false;

		hr = m_pDevice->SetRenderTarget(0, pSurface);
		hr |= m_pDevice->SetDepthStencilSurface( m_pDepthStencilTarget );
		if ( FAILED( hr ) )
			return false;
		pSurface->Release();
	}

	if(type & RT_Z)
	{
		IDirect3DSurface9* pZSurface = NULL;
		hr = m_pZTarget->GetSurfaceLevel( 0, &pZSurface );
		if ( FAILED( hr ) )
			return false;
		hr |= m_pDevice->SetRenderTarget(1, pZSurface);
		if ( FAILED( hr ) )
			return false;
		pZSurface->Release();
	}



	return hr == S_OK;
}

void RenderTarget::pop()
{
	if(m_type & RT_COLOR)
	{
		m_pDevice->SetRenderTarget(0, m_pLastRenderTarget);
		m_pDevice->SetDepthStencilSurface( m_pLastDepthStencilTarget );
	}
	if(m_type & RT_Z)
	{
		m_pDevice->SetRenderTarget(1, NULL);
	}
}

IDirect3DSurface9* RenderTarget::pSurface()
{
	IDirect3DSurface9* pSurface = NULL;
	HRESULT hr = m_pRenderTarget->GetSurfaceLevel( 0, &pSurface );
	pSurface->Release();

	if(FAILED(hr))
		return NULL;

	return pSurface;

}
