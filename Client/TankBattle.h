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

	bool isPlayer1 = true;

	TankBattle();

	~TankBattle();

	void update() override;

	void render();

private:
	boost::asio::io_context io_context_;
	boost::asio::ip::tcp::socket socket_;
	Transfer* transfer_;

	void handleNetworkCommunication();
};