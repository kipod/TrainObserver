#pragma once

typedef unsigned int uint;

struct Rect
{
	uint x;
	uint y;
	uint width;
	uint height;

	Rect(uint _x, uint _y, uint w, uint h) :
		x(_x), y(_y), width(w), height(h)
	{
	}

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
	virtual ~IRenderable() {};
	virtual void draw(class RendererDX9& renderer) = 0;
};


