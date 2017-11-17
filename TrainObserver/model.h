#pragma once
#include "render_interface.h"

class Geometry;
class Effect;

class Model : public IRenderable
{
public:
	Model();
	~Model();


	virtual void draw(class RendererDX9& renderer) override;

	void setup(Geometry* pGeometry, Effect* pEffect);
	void setTransform(const graph::Matrix& transform);
private:
	void setProperties(LPDIRECT3DDEVICE9 device);

private:
	Geometry*		m_geometry;
	Effect*			m_effect;
	graph::Matrix	m_transform;
};

