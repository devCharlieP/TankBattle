#pragma once

#include "Engine/Game2D.h"

class Bullet
{
public:
	vec2 center = vec2(0.0f, 0.0f);
	vec2 velocity = vec2(0.0f, 0.0f);

	Bullet();

	~Bullet();

	void draw();

	void update(const float& dt);
};