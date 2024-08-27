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

	//객체 렌더링
	void render();

	//서버 전송 및 수신 처리
	void handletransfer();

	//1p or 2p에 따른 탱크 위치 및 탄환 방향 처리
	void handle1p();

	// 탱크 탄환 충돌 처리 
	void handleTankCollision();

	// 탱크 투명 처리
	void handleTankVisibility(); 
};