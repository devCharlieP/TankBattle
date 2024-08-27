#include "Tank.h"

void Tank::draw()
{
	beginTransformation();
	{
		translate(center);
		drawFilledBox(Colors::green, 0.25f, 0.1f); // body
		translate(-0.02f * direction, 0.1f);
		drawFilledBox(Colors::blue, 0.15f, 0.09f); // turret
		translate(0.15f * direction, 0.0f);
		drawFilledBox(Colors::red, 0.15f, 0.03f);  // barrel
	}
	endTransformation();
}

void Tank::moveLeft(const float& dt)
{
	if (direction == 1.0f)
	{
		if (center.x > -1.7f)
		{
			center.x -= 0.5f * dt;
		}
	}
	else
	{
		if (center.x > 0.0f)
		{
			center.x -= 0.5f * dt;
		}
	}	
}

void Tank::moveRight(const float& dt)
{
	if (direction == 1.0f)
	{
		if (center.x < 0.0f)
		{
			center.x += 0.5f * dt;
		}
	}
	else
	{
		if (center.x < 1.7f)
		{
			center.x += 0.5f * dt;
		}
	}	
}

void Tank::moveUp(const float& dt)
{
	if (center.y < 0.9f)
	{
		center.y += 0.5f * dt;
	}
}

void Tank::moveDown(const float& dt)
{
	if (center.y > -1.0f)
	{
		center.y -= 0.5f * dt;
	}
}