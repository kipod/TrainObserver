#include "mesh.h"
#include "render_dx9.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::draw(class RendererDX9& renderer)
{
	if (m_effect)
	{
		auto pDevice = renderer.device();

		pDevice->SetFVF(m_fvf);		// Uses a different vertex so we must set it first
		pDevice->SetStreamSource(0, m_vb, 0, m_vertexSize);
		if (m_effect->begin())
		{
			for (UINT i = 0; i < m_effect->numPasses(); i++)
			{
				if (m_effect->beginPass(i))
				{
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
					m_effect->endPass();
				}
			}

			m_effect->end();
		}

	}
}
