#include "skybox.h"
#include "log_interface.h"
#include "render_dx9.h"
#include "vertex_formats.h"
#include "texture_manager.h"

// 24 vertices = 6 faces (cube) * 4 vertices per face
//
// Example diagram of "front" quad
// The numbers are vertices
// 
// 2  __________ 4
//   |\         |
//	 |  \       |
//   |    \     |
//   |      \   |
// 1 |        \ | 3
//	  ----------	 

XYZUV g_SkyboxMesh[24] =
{
	// Front quad, NOTE: All quads face inward
	{ Vector3(-10.0f, -10.0f,  10.0f),  0.0f, 1.0f },
	{ Vector3(-10.0f,  10.0f,  10.0f),  0.0f, 0.0f },
	{ Vector3(10.0f, -10.0f,  10.0f),  1.0f, 1.0f },
	{ Vector3(10.0f,  10.0f,  10.0f),  1.0f, 0.0f },

	// Back quad
	{ Vector3(10.0f, -10.0f, -10.0f),  0.0f, 1.0f },
	{ Vector3(10.0f,  10.0f, -10.0f),  0.0f, 0.0f },
	{ Vector3(-10.0f, -10.0f, -10.0f),  1.0f, 1.0f },
	{ Vector3(-10.0f,  10.0f, -10.0f),  1.0f, 0.0f },

	// Left quad
	{ Vector3(-10.0f, -10.0f, -10.0f),  0.0f, 1.0f },
	{ Vector3(-10.0f,  10.0f, -10.0f),  0.0f, 0.0f },
	{ Vector3(-10.0f, -10.0f,  10.0f),  1.0f, 1.0f },
	{ Vector3(-10.0f,  10.0f,  10.0f),  1.0f, 0.0f },

	// Right quad
	{ Vector3(10.0f, -10.0f,  10.0f),  0.0f, 1.0f },
	{ Vector3(10.0f,  10.0f,  10.0f),  0.0f, 0.0f },
	{ Vector3(10.0f, -10.0f, -10.0f),  1.0f, 1.0f },
	{ Vector3(10.0f,  10.0f, -10.0f),  1.0f, 0.0f },

	// Top quad
	{ Vector3(-10.0f,  10.0f,  10.0f),  0.0f, 1.0f },
	{ Vector3(-10.0f,  10.0f, -10.0f),  0.0f, 0.0f },
	{ Vector3(10.0f,  10.0f,  10.0f),  1.0f, 1.0f },
	{ Vector3(10.0f,  10.0f, -10.0f),  1.0f, 0.0f },

	// Bottom quad
	{ Vector3(-10.0f, -10.0f, -10.0f),  0.0f, 1.0f },
	{ Vector3(-10.0f, -10.0f,  10.0f),  0.0f, 0.0f },
	{ Vector3(10.0f, -10.0f, -10.0f),  1.0f, 1.0f },
	{ Vector3(10.0f, -10.0f,  10.0f),  1.0f, 0.0f }
};




SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
	if (m_vb)
	{
		m_vb->Release();
	}

	for (auto& texture : m_tiles)
	{
		texture->Release();
	}
}

bool SkyBox::create(RendererDX9& renderer, const char* textures[6])
{
	HRESULT hRet;

	auto pDevice = renderer.device();

	// Create our vertex buffer ( 24 vertices (4 verts * 6 faces) )
	hRet = pDevice->CreateVertexBuffer(sizeof(XYZUV) * 24, 0, D3DFVF_XYZ | D3DFVF_TEX1,
		D3DPOOL_MANAGED, &m_vb, NULL);
	if (FAILED(hRet))
	{
		LOG(MSG_ERROR, "Failed to create vertex buffer for skybox");
		return false;
	}

	void *pVertices = NULL;

	// Copy the skybox mesh into the vertex buffer.  I initialized the whole mesh array
	// above in global space.
	m_vb->Lock(0, sizeof(XYZUV) * 24, (void**)&pVertices, 0);
	memcpy(pVertices, g_SkyboxMesh, sizeof(XYZUV) * 24);
	m_vb->Unlock();

	// Load Textures.  I created a global array just to keep things simple.  The order of the images
	// is VERY important.  The reason is the skybox mesh (g_SkyboxMesh[]) array was created above
	// in this order. (ie. front, back, left, etc.)
	hRet = S_OK;
	for (int i = 0; i < 6; ++i)
	{
		LPDIRECT3DTEXTURE9 pTexture = RenderSystemDX9::instance().textureManager().get(textures[i]);
		if (!FAILED(hRet))
		{
			m_tiles.emplace_back(pTexture);
		}
		else
		{
			LOG(MSG_ERROR, "Couldn't load texture %s for skybox", textures[i]);
			return false;
		}
	}

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	// If we get here then we succeeded!
	return true;
}

void SkyBox::draw(RendererDX9& renderer)
{
	auto pDevice = renderer.device();

	pDevice->SetTransform(D3DTS_VIEW, &renderer.camera().view());
	pDevice->SetTransform(D3DTS_PROJECTION, &renderer.camera().projection());

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// Render the sky box
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	pDevice->SetStreamSource(0, m_vb, 0, sizeof(XYZUV));

	// Set the world matrix to identity for the skybox
	D3DXMATRIX oldWorld;
	pDevice->GetTransform(D3DTS_WORLD, &oldWorld);
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	pDevice->SetTransform(D3DTS_WORLD, &world);

	Camera& camera = renderer.camera();
	Vector3 camPos(camera.pos());
	camera.pos(Vector3(0, 0, 0));

	pDevice->SetTransform(D3DTS_VIEW, &camera.view());

	camera.pos(camPos);


	// Render the 6 faces of the skybox
	// DrawPrimitive is used to draw polygons when the vertices are stored in a device resource 
	// called a vertex buffer. Vertex buffers are blocks of memory allocated by the device that
	// we use to store vertex data.
	for (ULONG i = 0; i < 6; ++i)
	{
		// Set the texture for this primitive
		pDevice->SetTexture(0, m_tiles[i]);

		// Render the face (one strip per face from the vertex buffer)  There are 2 primitives per face.
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);

	} // Next side

	pDevice->SetTransform(D3DTS_WORLD, &oldWorld);
}
