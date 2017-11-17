#include "box.h"
#include "vertex_formats.h"
#include <vector>

XYZUV g_vertices[8] = 
{
	// down quad
	{ graph::Vector3(-0.5f, -0.5f,  -0.5f),  0.0f, 0.0f },
	{ graph::Vector3(-0.5f,  0.5f,  -0.5f),  0.0f, 1.0f },
	{ graph::Vector3( 0.5f,	 0.5f,	-0.5f),  1.0f, 1.0f },
	{ graph::Vector3( 0.5f,	-0.5f,	-0.5f),  1.0f, 0.0f },

	// up quad
	{ graph::Vector3(-0.5f, -0.5f,  0.5f),  0.0f, 0.0f },
	{ graph::Vector3(-0.5f,  0.5f,  0.5f),  0.0f, 1.0f },
	{ graph::Vector3( 0.5f,	 0.5f,	0.5f),  1.0f, 1.0f },
	{ graph::Vector3( 0.5f,	-0.5f,	0.5f),  1.0f, 0.0f },
};

unsigned short g_indices[36] = 
{
	//bottom
	0,3,1,
	1,3,2,
	//back
	0,3,4,
	4,3,7,
	//front
	2,5,6,
	2,1,5,
	//right
	3,6,7,
	3,2,6,
	//left
	1,0,5,
	0,4,5,
	//up
	4,7,5,
	7,6,5
};


XYZUV g_boxMesh[24] =
{
	// Front quad, NOTE: All quads face inward
	{ graph::Vector3(-0.5f, -0.5f,  0.5f),  0.0f, 0.5f },
	{ graph::Vector3(-0.5f,  0.5f,  0.5f),  0.0f, 0.0f },
	{ graph::Vector3(0.5f, -0.5f,  0.5f),  0.5f, 0.5f },
	{ graph::Vector3(0.5f,  0.5f,  0.5f),  0.5f, 0.0f },

	// Back quad
	{ graph::Vector3(0.5f, -0.5f, -0.5f),  0.0f, 0.5f },
	{ graph::Vector3(0.5f,  0.5f, -0.5f),  0.0f, 0.0f },
	{ graph::Vector3(-0.5f, -0.5f, -0.5f),  0.5f, 0.5f },
	{ graph::Vector3(-0.5f,  0.5f, -0.5f),  0.5f, 0.0f },

	// Left quad
	{ graph::Vector3(-0.5f, -0.5f, -0.5f),  0.0f, 0.5f },
	{ graph::Vector3(-0.5f,  0.5f, -0.5f),  0.0f, 0.0f },
	{ graph::Vector3(-0.5f, -0.5f,  0.5f),  0.5f, 0.5f },
	{ graph::Vector3(-0.5f,  0.5f,  0.5f),  0.5f, 0.0f },

	// Right quad
	{ graph::Vector3(0.5f, -0.5f,  0.5f),  0.0f, 0.5f },
	{ graph::Vector3(0.5f,  0.5f,  0.5f),  0.0f, 0.0f },
	{ graph::Vector3(0.5f, -0.5f, -0.5f),  0.5f, 0.5f },
	{ graph::Vector3(0.5f,  0.5f, -0.5f),  0.5f, 0.0f },

	// Top quad
	{ graph::Vector3(-0.5f,  0.5f,  0.5f),  0.0f, 0.5f },
	{ graph::Vector3(-0.5f,  0.5f, -0.5f),  0.0f, 0.0f },
	{ graph::Vector3(0.5f,  0.5f,  0.5f),  0.5f, 0.5f },
	{ graph::Vector3(0.5f,  0.5f, -0.5f),  0.5f, 0.0f },

	// Bottom quad
	{ graph::Vector3(-0.5f, -0.5f, -0.5f),  0.0f, 0.5f },
	{ graph::Vector3(-0.5f, -0.5f,  0.5f),  0.0f, 0.0f },
	{ graph::Vector3(0.5f, -0.5f, -0.5f),  0.5f, 0.5f },
	{ graph::Vector3(0.5f, -0.5f,  0.5f),  0.5f, 0.0f }
};


Box::Box()
{

}

Box::~Box()
{

}

void fillVertices(std::vector<XYZUV>& vertices, std::vector<unsigned short>& indices)
{
	vertices.reserve(8);
	for (auto& v : g_vertices)
	{
		vertices.emplace_back(v);
	}

	indices.reserve(36);
	for (auto& v : g_indices)
	{
		indices.emplace_back(v);
	}
}

bool Box::create(LPDIRECT3DDEVICE9 pDevice, const graph::Vector3& size)
{
	static std::vector<XYZUV> vertices;
	static std::vector<unsigned short> indices;
	if (vertices.empty() && indices.empty())
	{
		fillVertices(vertices, indices);
	}

	if (Geometry::create<XYZUV, unsigned short>(pDevice, vertices, indices))
	{
		m_size = size;
		return true;
	}

	return false;
}


