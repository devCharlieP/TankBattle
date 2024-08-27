#pragma once

#include "Engine/Game2D.h"
#include "Wall.h"
#include "Bullet.h"
#include "Tank.h"
#include "Transfer.h"
#include <boost/asio.hpp>
#include <vector>

class Transfer;

class TankBattle : public Game2D
{
public:
	Wall Wall;

	Tank MyTank;
	Tank EnemyTank;

	std::vector<Bullet*> Mybullets;
	std::vector<Bullet*> Enemybullets;

	boost::asio::io_context io_context;
	Transfer transfer;

	bool status;
	bool is1p;

	TankBattle();
	~TankBattle();

	void update() override;
	void render();

	void handletransfer();
	void handle1p();
};