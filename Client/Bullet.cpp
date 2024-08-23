#include "Bullet.h"

Bullet::Bullet()
{
	
}

Bullet::~Bullet()
{
	
}

void Bullet::draw()
{
	beginTransformation();
	translate(center);
	drawFilledRegularConvexPolygon(Colors::yellow, 0.02f, 8);
	drawWiredRegularConvexPolygon(Colors::gray, 0.02f, 8);
	endTransformation();
}

void Bullet::update(const float& dt)
{
	center += velocity * dt * 0.5f;
}