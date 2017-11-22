#pragma once
#include "render_interface.h"
#include "effect.h"
#include <memory>
#include "log.h"

struct PrimitiveGroup
{
	uint vertexOffset = 0;
	uint indexOffset = 0;
	uint nTriangles = 0;
	EffectProperties properties;
};

typedef std::vector<PrimitiveGroup> PrimitiveGroups;

class Geometry
{
public:
	Geometry();
	~Geometry();

	template<class VertexType, class IndexType>
	bool create(
		LPDIRECT3DDEVICE9 pDevice, 
		std::vector<VertexType>& vertices,
		bool normalizeSize = true,
		const PrimitiveGroups* primitiveGroups = nullptr,
		const std::vector<IndexType>* indices = nullptr	);

	static Geometry* create(const std::string& path, bool normalizeSize = true);

	void draw(LPDIRECT3DDEVICE9 pDevice, Effect& effect);

private:
	template<class VertexType>
	void normalize(std::vector<VertexType>& vertices);

private:
	LPDIRECT3DVERTEXBUFFER9			m_vb;
	LPDIRECT3DINDEXBUFFER9			m_ib;
	DWORD							m_fvf;
	uint							m_vertexSize;
	uint							m_nTriangles;
	uint							m_nVertices;
	PrimitiveGroups					m_primitiveGroups;

	LPD3DXMESH						m_mesh;

};

template<class VertexType, class IndexType>
bool Geometry::create(
	LPDIRECT3DDEVICE9 pDevice, 
	std::vector<VertexType>& vertices,
	bool normalizeSize,
	const PrimitiveGroups* primitiveGroups,
	const std::vector<IndexType>* indices )
{
	HRESULT hRet;

	if (normalizeSize)
	{
		normalize(vertices);
	}

	m_nTriangles = indices ? indices->size() / 3 : 0;

	if (primitiveGroups)
	{
		m_primitiveGroups = *primitiveGroups;
	}
	else
	{
		PrimitiveGroup primGroup;
		primGroup.indexOffset = 0;
		primGroup.vertexOffset = 0;
		primGroup.nTriangles = m_nTriangles;
		m_primitiveGroups.emplace_back(primGroup);
	}

	m_nVertices = vertices.size();
	m_vertexSize = sizeof(VertexType);
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

	// Temporary vertex array pointer
	void *pData = NULL;

	m_vb->Lock(0, sizeInBytes, (void**)&pData, 0);
	memcpy_s(pData, sizeInBytes, vertices.data(), sizeInBytes);
	m_vb->Unlock();

	if (indices)
	{
		// Create our vertex buffer
		hRet = pDevice->CreateIndexBuffer(
			indices->size() * sizeof(IndexType), 
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

		sizeInBytes = sizeof(IndexType) * indices->size();

		m_ib->Lock(0, sizeInBytes, (void**)&pData, 0);
		memcpy_s(pData, sizeInBytes, indices->data(), sizeInBytes);
		m_ib->Unlock();
	}

	return true;
}

template<class VertexType>
void Geometry::normalize(std::vector<VertexType>& vertices)
{
	graph::Vector3 vmax(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
	graph::Vector3 vmin(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

	for (const auto& v : vertices)
	{
		if (v.pos.x < vmin.x)
			vmin.x = v.pos.x;
		if (v.pos.y < vmin.y)
			vmin.y = v.pos.y;
		if (v.pos.z < vmin.z)
			vmin.z = v.pos.z;

		if (v.pos.x > vmax.x)
			vmax.x = v.pos.x;
		if (v.pos.y > vmax.y)
			vmax.y = v.pos.y;
		if (v.pos.z > vmax.z)
			vmax.z = v.pos.z;
	}

	graph::Vector3 delta(vmax - vmin);
	graph::Vector3 center((vmin + vmax) * 0.5f);
	float deltaf = max(delta.x, max(delta.y, delta.z));

	delta = graph::Vector3(0.0f, delta.y / 2.0f, 0.0f);

	for (auto& v : vertices)
	{
		v.pos = (v.pos - center + delta) / deltaf;
	}
}

