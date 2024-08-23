#pragma once

#include "Engine/Game2D.h"

class Wall
{
public:

	Wall()
	{

	}

	~Wall()
	{

	}

	void draw()
	{
		beginTransformation();

		translate(0.0f, 0.0f);
		drawFilledBox(Colors::black, 0.02f, 2.0f);

		endTransformation();
	}
};