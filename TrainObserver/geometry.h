#pragma once
#include "render_interface.h"
#include "effect.h"
#include <memory>
#include "log.h"

class Geometry
{
public:
	Geometry();
	~Geometry();

	template<class VertexType, class IndexType>
	bool create(LPDIRECT3DDEVICE9 pDevice, const std::vector<VertexType>& vertices, const std::vector<IndexType>& indices);

	LPDIRECT3DVERTEXBUFFER9 vb() const;
	LPDIRECT3DINDEXBUFFER9 ib() const;
	DWORD fvf() const;
	uint vertexSize() const;
	uint numVertices() const;
	uint numTriangles() const;
private:
	LPDIRECT3DVERTEXBUFFER9			m_vb;
	LPDIRECT3DINDEXBUFFER9			m_ib;
	DWORD							m_fvf;
	uint							m_vertexSize;
	uint							m_nTriangles;
	uint							m_nVertices;
};

template<class VertexType, class IndexType>
bool Geometry::create(LPDIRECT3DDEVICE9 pDevice, const std::vector<VertexType>& vertices, const std::vector<IndexType>& indices)
{
	HRESULT hRet;

	m_nVertices = vertices.size();
	m_vertexSize = sizeof(VertexType);
	m_nTriangles = indices.size() / 3;
	m_fvf = VertexType::fvf();

	uint sizeInBytes = m_vertexSize * m_nVertices;

	// Create our vertex buffer
	hRet = pDevice->CreateVertexBuffer(
		sizeInBytes, 
		0, 
		VertexType::fvf(),
		D3DPOOL_MANAGED, 
		&m_vb, 
		NULL);
	
	if (FAILED(hRet))
	{
		LOG(MSG_ERROR, "Failed to create vertex buffer");
		return false;
	}

	// Create our vertex buffer
	hRet = pDevice->CreateIndexBuffer(
		indices.size() * sizeof(IndexType), 
		0, 
		sizeof(IndexType) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
		D3DPOOL_MANAGED, 
		&m_ib, 
		NULL);

	if (FAILED(hRet))
	{
		LOG(MSG_ERROR, "Failed to create vertex buffer");
		return false;
	}


	// Temporary vertex array pointer
	void *pData = NULL;

	m_vb->Lock(0, sizeInBytes, (void**)&pData, 0);
	memcpy_s(pData, sizeInBytes, vertices.data(), sizeInBytes);
	m_vb->Unlock();

	sizeInBytes = sizeof(IndexType) * indices.size();

	m_ib->Lock(0, sizeInBytes, (void**)&pData, 0);
	memcpy_s(pData, sizeInBytes, indices.data(), sizeInBytes);
	m_ib->Unlock();



	return true;
}

