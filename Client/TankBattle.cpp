#include "TankBattle.h"

TankBattle::TankBattle() : Game2D("This is my digital canvas!", 1920, 1080, false, 2)
{

}

TankBattle::~TankBattle()
{
	for (auto bullet : bullets) {
		delete bullet;
	}	

	bullets.clear();
}

void TankBattle::update()
{
	// move tank
	bool is_moving = false;

	if (isKeyPressed(GLFW_KEY_LEFT)) {
		tank.moveLeft(getTimeStep());
		is_moving = true;
	}

	if (isKeyPressed(GLFW_KEY_RIGHT)) {
		tank.moveRight(getTimeStep());
		is_moving = true;
	}

	if (isKeyPressed(GLFW_KEY_UP)) {
		tank.moveUp(getTimeStep());
		is_moving = true;
	}

	if (isKeyPressed(GLFW_KEY_DOWN)) {
		tank.moveDown(getTimeStep());
		is_moving = true;
	}

	// shoot a cannon ball
	if (isKeyPressedAndReleased(GLFW_KEY_SPACE))
	{
		Bullet* NewBullet = new Bullet;

		NewBullet->center = tank.center;
		NewBullet->center.x += 0.2f;
		NewBullet->center.y += 0.1f;
		NewBullet->velocity = vec2(2.0f, 0.0f);

		bullets.push_back(NewBullet);
	}

	for (auto bullet : bullets) {
		bullet->update(getTimeStep());
	}

	// remove bullets that go out of the screen
	bullets.erase(
		std::remove_if(bullets.begin(), bullets.end(), [](Bullet* bullet) {
			bool shouldDelete = bullet->center.x > 2.0f || bullet->center.x < -2.0f || bullet->center.y > 2.0f || bullet->center.y < -2.0f;
			
			if (shouldDelete) {
				delete bullet;
			}

			return shouldDelete;
			}),

		bullets.end()
	);

	// rendering
	tank.draw();
	for (auto bullet : bullets) {
		bullet->draw();
	}
}