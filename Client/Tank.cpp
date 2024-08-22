#include "Tank.h"

void Tank::draw()
{
	beginTransformation();
	{
		translate(center);
		drawFilledBox(Colors::green, 0.25f, 0.1f); // body
		translate(-0.02f, 0.1f);
		drawFilledBox(Colors::blue, 0.15f, 0.09f); // turret
		translate(0.15f, 0.0f);
		drawFilledBox(Colors::red, 0.15f, 0.03f);  // barrel
	}
	endTransformation();
}

void Tank::moveLeft(const float& dt)
{
	center.x -= 0.5f * dt;
}

void Tank::moveRight(const float& dt)
{
	center.x += 0.5f * dt;
}

void Tank::moveUp(const float& dt)
{
	center.y += 0.5f * dt;
}

void Tank::moveDown(const float& dt)
{
	center.y -= 0.5f * dt;
}