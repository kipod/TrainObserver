#include "model.h"
#include "render_dx9.h"
#include "effect.h"
#include "log.h"
#include "geometry.h"



Model::Model():
	m_effectProperties(new EffectProperties())
{
	m_transform.id();
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

	setProperties();

	m_geometry->draw(pDevice, *m_effect);
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

EffectProperties& Model::effectProperties()
{
	return *m_effectProperties.get();
}

void Model::setProperties()
{
	m_effectProperties->setMatrix("World", m_transform);
	m_effectProperties->applyProperties(m_effect);
}
