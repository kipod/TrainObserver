#include "box.h"
#include "vertex_formats.h"



Box::Box()
{
}


Box::~Box()
{
}


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


bool Box::create(float width, float length, float height)
{

}

void Box::draw(class RendererDX9& renderer)
{

}
