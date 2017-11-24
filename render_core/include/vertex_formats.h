#pragma once
#include "math\vector3.h"

struct XYZUV
{
	Vector3 pos;		
	float u, v;

	static unsigned int fvf()
	{
		return D3DFVF_XYZ | D3DFVF_TEX1;
	}
};

struct XYZNUV
{
	Vector3 pos;
	Vector3 normal;
	float u, v;

	static unsigned int fvf()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	}
};

struct XYZNUVTB
{
	Vector3 pos;
	Vector3 normal;
	float u, v;
	Vector3 tangent;
	Vector3 binormal;

	static unsigned int fvf()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3;
	}
};


