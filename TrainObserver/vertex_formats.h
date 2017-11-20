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

