#pragma once
#include "geometry.h"



class Box: public Geometry
{
public:
	Box();
	~Box();

	bool create(LPDIRECT3DDEVICE9 pDevice);
};

