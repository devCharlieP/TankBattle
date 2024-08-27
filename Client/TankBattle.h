#pragma once

#include "Engine/Game2D.h"
#include "Wall.h"
#include "Tank.h"
#include "Bullet.h"
#include "Transfer.h"
#include <boost/asio.hpp>
#include <vector>
#include <chrono>

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

	bool isMyTankVisible = true;
	bool isEnemyTankVisible = true;
	std::chrono::steady_clock::time_point hitTime;
	std::chrono::steady_clock::time_point enemyHitTime;

	TankBattle();
	~TankBattle();

	void update() override;

	//��ü ������
	void render();

	//���� ���� �� ���� ó��
	void handletransfer();

	//1p or 2p�� ���� ��ũ ��ġ �� źȯ ���� ó��
	void handle1p();

	// ��ũ źȯ �浹 ó�� 
	void handleTankCollision();

	// ��ũ ���� ó��
	void handleTankVisibility(); 
};