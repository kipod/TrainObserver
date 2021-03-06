#include "box.h"
#include "vertex_formats.h"
#include <vector>
#include "geometry_utils.h"

typedef XYZNUVTB Vertex;

XYZNUV g_vertices[24] = 
{
	// down quad
	{ Vector3(-0.5f, -0.5f,  -0.5f), Vector3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f },	//0		0
	{ Vector3(-0.5f,  0.5f,  -0.5f), Vector3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f },	//1		1
	{ Vector3(0.5f,	 0.5f,	-0.5f), Vector3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f },	//2		2
	{ Vector3(0.5f,	-0.5f,	-0.5f), Vector3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f },	//3		3

	// back quad
	{ Vector3(-0.5f, -0.5f,  -0.5f), Vector3(0.0f, -1.0f,	0.0f), 0.0f, 0.0f },	//0		4
	{ Vector3(0.5f,	-0.5f,	-0.5f), Vector3(0.0f, -1.0f,	0.0f), 0.0f, 1.0f },	//3		5
	{ Vector3(-0.5f, -0.5f,  0.5f),	Vector3(0.0f, -1.0f,	0.0f), 1.0f, 0.0f },	//4		6
	{ Vector3(0.5f,	-0.5f,	0.5f),	Vector3(0.0f, -1.0f,	0.0f), 1.0f, 1.0f },	//7		7

	// front quad
	{ Vector3(0.5f,	 0.5f,	-0.5f), Vector3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f },	//2		8
	{ Vector3(-0.5f,  0.5f,  0.5f),  Vector3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f },	//5		9
	{ Vector3(0.5f,	 0.5f,	0.5f),	Vector3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f },	//6		10
	{ Vector3(-0.5f,  0.5f,  -0.5f),	Vector3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f },	//1		11

	// right quad
	{ Vector3(0.5f,	-0.5f,	-0.5f), Vector3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f },	//3		12
	{ Vector3(0.5f,	 0.5f,	0.5f),	Vector3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f },	//6		13
	{ Vector3(0.5f,	-0.5f,	0.5f),	Vector3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f },	//7		14
	{ Vector3(0.5f,	 0.5f,	-0.5f), Vector3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f },	//2		15

	// left quad
	{ Vector3(-0.5f, -0.5f,  -0.5f), Vector3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f },	//0		16
	{ Vector3(-0.5f,  0.5f,  -0.5f), Vector3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f },	//1		17
	{ Vector3(-0.5f, -0.5f,  0.5f),  Vector3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f },	//4		18
	{ Vector3(-0.5f,  0.5f,  0.5f),  Vector3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f },	//5		19

	// up quad
	{ Vector3(-0.5f, -0.5f,  0.5f), Vector3(0.0f,	0.0f, 1.0f), 0.0f, 0.0f },	//4		20
	{ Vector3(-0.5f,  0.5f,  0.5f), Vector3(0.0f,	0.0f, 1.0f), 0.0f, 1.0f },	//5		21
	{ Vector3(0.5f,	 0.5f,	0.5f), Vector3(0.0f,	0.0f, 1.0f), 1.0f, 1.0f },	//6		22
	{ Vector3(0.5f,	-0.5f,	0.5f), Vector3(0.0f,	0.0f, 1.0f), 1.0f, 0.0f },	//7		23
};

unsigned short g_indices[36] = 
{
	//bottom
	0,1,3,
	1,2,3,
	//back
	4,5,6,
	6,5,7,
	//front
	8,9,10,
	8,11,9,
	//right
	12,13,14,
	12,15,13,
	//left
	17,16,19,
	16,18,19,
	//up
	20,23,21,
	23,22,21
};


XYZUV g_boxMesh[24] =
{
	// Front quad, NOTE: All quads face inward
	{ Vector3(-0.5f, -0.5f,  0.5f),  0.0f, 0.5f },
	{ Vector3(-0.5f,  0.5f,  0.5f),  0.0f, 0.0f },
	{ Vector3(0.5f, -0.5f,  0.5f),  0.5f, 0.5f },
	{ Vector3(0.5f,  0.5f,  0.5f),  0.5f, 0.0f },

	// Back quad
	{ Vector3(0.5f, -0.5f, -0.5f),  0.0f, 0.5f },
	{ Vector3(0.5f,  0.5f, -0.5f),  0.0f, 0.0f },
	{ Vector3(-0.5f, -0.5f, -0.5f),  0.5f, 0.5f },
	{ Vector3(-0.5f,  0.5f, -0.5f),  0.5f, 0.0f },

	// Left quad
	{ Vector3(-0.5f, -0.5f, -0.5f),  0.0f, 0.5f },
	{ Vector3(-0.5f,  0.5f, -0.5f),  0.0f, 0.0f },
	{ Vector3(-0.5f, -0.5f,  0.5f),  0.5f, 0.5f },
	{ Vector3(-0.5f,  0.5f,  0.5f),  0.5f, 0.0f },

	// Right quad
	{ Vector3(0.5f, -0.5f,  0.5f),  0.0f, 0.5f },
	{ Vector3(0.5f,  0.5f,  0.5f),  0.0f, 0.0f },
	{ Vector3(0.5f, -0.5f, -0.5f),  0.5f, 0.5f },
	{ Vector3(0.5f,  0.5f, -0.5f),  0.5f, 0.0f },

	// Top quad
	{ Vector3(-0.5f,  0.5f,  0.5f),  0.0f, 0.5f },
	{ Vector3(-0.5f,  0.5f, -0.5f),  0.0f, 0.0f },
	{ Vector3(0.5f,  0.5f,  0.5f),  0.5f, 0.5f },
	{ Vector3(0.5f,  0.5f, -0.5f),  0.5f, 0.0f },

	// Bottom quad
	{ Vector3(-0.5f, -0.5f, -0.5f),  0.0f, 0.5f },
	{ Vector3(-0.5f, -0.5f,  0.5f),  0.0f, 0.0f },
	{ Vector3(0.5f, -0.5f, -0.5f),  0.5f, 0.5f },
	{ Vector3(0.5f, -0.5f,  0.5f),  0.5f, 0.0f }
};


Box::Box()
{

}

Box::~Box()
{

}

void XYZNUVtoXYZNUVTB(const XYZNUV& in, XYZNUVTB& out)
{
	out.pos = in.pos;
	out.normal = in.normal;
	out.u = in.u;
	out.v = in.v;

	generateTangentAndBinormal(out);
}

void fillVertices(std::vector<Vertex>& vertices, std::vector<unsigned short>& indices)
{
	vertices.reserve(24);
	for (auto& v : g_vertices)
	{
		v.normal.Normalize();
		XYZNUVTB vtx;
		XYZNUVtoXYZNUVTB(v, vtx);
		vertices.emplace_back(vtx);
	}

	indices.reserve(36);
	for (auto& v : g_indices)
	{
		indices.emplace_back(v);
	}
}

bool Box::create(LPDIRECT3DDEVICE9 pDevice)
{
	static std::vector<Vertex> vertices;
	static std::vector<unsigned short> indices;
	if (vertices.empty() && indices.empty())
	{
		fillVertices(vertices, indices);
	}

	if (Geometry::create(pDevice, vertices, false, &indices))
	{
		return true;
	}

	return false;
}


