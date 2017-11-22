#pragma once
#include "render_interface.h"
#include <memory>

class Geometry;
class Effect;
class EffectProperties;


class Model : public IRenderable
{
public:
	Model();
	virtual ~Model();


	virtual void draw(class RendererDX9& renderer) override;

	void setup(Geometry* pGeometry, Effect* pEffect);
	void setTransform(const graph::Matrix& transform);

	EffectProperties& effectProperties();
private:
	void setProperties();

private:
	Geometry*							m_geometry;
	Effect*								m_effect;
	graph::Matrix						m_transform;
	std::unique_ptr<EffectProperties>	m_effectProperties;
};

