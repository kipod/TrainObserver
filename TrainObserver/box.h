#pragma once
#include "render_interface.h"



class Box: public IRenderable
{
public:
	Box();
	~Box();

	bool create(float width, float length, float height);


};

