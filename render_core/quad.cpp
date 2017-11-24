#include "quad.h"
#include "vertex_formats.h"

static XYZUV g_vertices[4] = 
{
	{ Vector3(-1.0f, -1.0f,	0.0f),	0.0f, 1.0f },
	{ Vector3(-1.0f,  1.0f,	0.0f),	0.0f, 0.0f },
	{ Vector3(1.0f,  1.0f,	0.0f),	1.0f, 0.0f },
	{ Vector3(1.0f, -1.0f,	0.0f),	1.0f, 1.0f }
};

static unsigned short g_indices[6] = 
{
	0,1,2,
	0,2,3
};


Quad::Quad()
{
}


Quad::~Quad()
{
}

void fillVertices(std::vector<XYZUV>& vertices, std::vector<unsigned short>& indices)
{
	vertices.reserve(4);
	for (auto& v : g_vertices)
	{
		vertices.emplace_back(v);
	}

	indices.reserve(6);
	for (auto& v : g_indices)
	{
		indices.emplace_back(v);
	}
}


bool Quad::create(LPDIRECT3DDEVICE9 pDevice)
{
	std::vector<XYZUV> vertices;
	std::vector<unsigned short> indices;

	fillVertices(vertices, indices);

	if (Geometry::create(pDevice, vertices, false, &indices))
	{
		return true;
	}

	return false;
}
