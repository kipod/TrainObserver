#include "model.h"
#include "render_dx9.h"
#include "effect.h"
#include "log.h"
#include "geometry.h"



Model::Model()
{
	m_transform.SetTranslation(graph::Vector3(0, 0, 0));
}


Model::~Model()
{
}

void Model::draw(RendererDX9& renderer)
{
	if (!m_effect || !m_geometry)
	{
		LOG(MSG_ERROR, "Model is not ready for drawing");
		return;
	}

	auto pDevice = renderer.device();

	pDevice->SetFVF(m_geometry->fvf());	
	pDevice->SetStreamSource(0, m_geometry->vb(), 0, m_geometry->vertexSize());
	pDevice->SetIndices(m_geometry->ib());

	if (m_effect->begin())
	{
		for (UINT i = 0; i < m_effect->numPasses(); i++)
		{
			if (m_effect->beginPass(i))
			{
				pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST, 
					0, 
					0, 
					m_geometry->numVertices(),
					0,
					m_geometry->numTriangles());
				m_effect->endPass();
			}
		}

		m_effect->end();
	}
}

void Model::setup(Geometry* pGeometry, Effect* pEffect)
{
	m_geometry = pGeometry;
	m_effect = pEffect;
}

void Model::setTransform(const graph::Matrix& transform)
{
	m_transform = transform;
}

void Model::setProperties(LPDIRECT3DDEVICE9 device)
{
	m_effect->setProperty(device, "World", m_transform);
}
