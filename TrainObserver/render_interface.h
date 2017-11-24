#pragma once
#include "defs.hpp"
#include "camera.h"
#include <memory>

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

class ITickable
{
public:
	virtual void tick(float deltaTime) = 0;
};

class IInputListener
{
public:
	virtual void onMouseMove(int x, int y, int delta_x, int delta_y, bool bLeftButton) {};
	virtual void onMouseWheel(int nMouseWheelDelta) {};
	virtual void processInput(float deltaTime, unsigned char keys[256]) {};
};

class IRenderer : public IInputListener
{
public:
	Camera& camera() { return m_camera; }

	virtual void draw() = 0;

protected:
	Camera								m_camera;
};

class IRenderSystem
{
public:
	virtual HRESULT init(HWND hwnd) = 0;
	virtual void fini() = 0;

	//virtual IRenderer& renderer() = 0;
};

