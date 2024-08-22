#pragma once

#include "Engine/Game2D.h"
#include "Bullet.h"
#include "Tank.h"
#include <vector>

class TankBattle : public Game2D
{
public:
	Tank tank;

	std::vector<Bullet*> bullets;

public:
	TankBattle();

	~TankBattle();

	void update() override;

};