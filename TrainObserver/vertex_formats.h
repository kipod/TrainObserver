#pragma once
#include "math\Vector3.h"

struct XYZUV
{
	graph::Vector3 pos;		
	float u, v;

	static uint fvf()
	{
		return D3DFVF_XYZ | D3DFVF_TEX1;
	}
};
