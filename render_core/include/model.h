#pragma once
#include "render_interface.h"
#include <memory>
#include "math\matrix.h"

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
	void setTransform(const Matrix& transform);

	EffectProperties& effectProperties();
private:
	void setProperties();

private:
	Geometry*							m_geometry;
	Effect*								m_effect;
	Matrix								m_transform;
	std::unique_ptr<EffectProperties>	m_effectProperties;
};

