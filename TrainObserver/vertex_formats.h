#pragma once
#include "defs.hpp"
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

struct XYZNUV
{
	graph::Vector3 pos;
	graph::Vector3 normal;
	float u, v;

	static uint fvf()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	}
};

struct XYZNUVTB
{
	graph::Vector3 pos;
	graph::Vector3 normal;
	float u, v;
	graph::Vector3 tangent;
	graph::Vector3 binormal;

	static uint fvf()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3;
	}
};


