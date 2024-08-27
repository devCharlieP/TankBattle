#pragma once

#include "Engine/Game2D.h"

class Tank
{
public:
	vec2 center = vec2(0.0f, 0.0f);
	float direction = 0.0f;

	void draw();

	void moveLeft(const float& dt);

	void moveRight(const float& dt);

	void moveUp(const float& dt);

	void moveDown(const float& dt);
};