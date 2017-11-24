#pragma once

typedef unsigned int uint;

struct Rect
{
	uint x;
	uint y;
	uint width;
	uint height;
};

struct Point
{
	float x;
	float y;
};

enum EObjectType
{
	CITY,
	RAIL
};

class IRenderable
{
public:
	virtual void draw(class RendererDX9& renderer) = 0;
};


