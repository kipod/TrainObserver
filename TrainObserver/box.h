#pragma once
#include "render_interface.h"



class Box: public IRenderable
{
public:
	Box();
	~Box();

	bool create(float width, float length, float height);

	virtual void draw(class RendererDX9& renderer) override;

private:
	LPDIRECT3DVERTEXBUFFER9 m_vb;
};

