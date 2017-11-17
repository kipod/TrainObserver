#include "geometry.h"
#include "render_dx9.h"



Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

LPDIRECT3DVERTEXBUFFER9 Geometry::vb() const
{
	return m_vb;
}

LPDIRECT3DINDEXBUFFER9 Geometry::ib() const
{
	return m_ib;
}

DWORD Geometry::fvf() const
{
	return m_fvf;
}

uint Geometry::vertexSize() const
{
	return m_vertexSize;
}

uint Geometry::numVertices() const
{
	return m_nVertices;
}

uint Geometry::numTriangles() const
{
	return m_nTriangles;
}


