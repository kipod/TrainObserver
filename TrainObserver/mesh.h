#pragma once
#include "render_interface.h"
#include "effect.h"
#include <memory>

class Mesh: public IRenderable
{
public:
	Mesh();
	~Mesh();

	template<class VertexType>
	bool create(LPDIRECT3DDEVICE9 pDevice, const VertexType* vertices, uint nVerts, Effect* pEffect);

	virtual void draw(class RendererDX9& renderer) override;

private:
	LPDIRECT3DVERTEXBUFFER9			m_vb;
	DWORD							m_fvf;
	uint							m_vertexSize;
	std::unique_ptr<class Effect>	m_effect;
};

template<class VertexType>
bool Mesh::create(LPDIRECT3DDEVICE9 pDevice, const VertexType* vertices, uint nVerts, Effect* pEffect)
{
	HRESULT hRet;

	uint sizeInBytes = sizeof(VertexType) * nVerts;

	// Create our vertex buffer
	hRet = pDevice->CreateVertexBuffer(sizeInBytes, 0, VertexType::fvf(),
		D3DPOOL_MANAGED, &m_vb, NULL);
	
	if (FAILED(hRet))
	{
		LOG(MSG_ERROR, "Failed to create vertex buffer");
		return false;
	}

	// Temporary vertex array pointer
	void *pVertices = NULL;

	// Copy the mesh into the vertex buffer.  I initialized the whole mesh array
	// above in global space.
	m_vb->Lock(0, sizeInBytes, (void**)&pVertices, 0);
	memcpy_s(pVertices, sizeInBytes, vertices, sizeInBytes);
	m_vb->Unlock();

	m_fvf = VertexType::fvf();
	m_vertexSize = sizeof(VertexType);

	m_effect = pEffect;

	return m_effect != nullptr;
}

