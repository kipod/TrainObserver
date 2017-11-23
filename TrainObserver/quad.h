#pragma once
#include "geometry.h"



class Quad : public Geometry
{
public:
	Quad();
	virtual ~Quad();

	bool create(LPDIRECT3DDEVICE9 pDevice);
};

