#include "TankBattle.h"

TankBattle::TankBattle() : Game2D("This is my digital canvas!", 1920, 1080, true, 2), socket_(io_context_)
{
	try {
		boost::asio::ip::tcp::resolver resolver(io_context_);
		boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
		boost::asio::connect(socket_, endpoints);

		transfer_ = new Transfer(socket_);
		isPlayer1 = transfer_->receivePlayerInfo();
	}
	catch (std::exception& e) {
		std::cerr << "Could not connect to server: " << e.what() << std::endl;

		transfer_ = nullptr;
		isPlayer1 = true;
	}

	// 탱크의 초기 위치 설정
	if (isPlayer1)
	{
		MyTank.center = vec2(-0.5f, 0.0f);
	}
	else 
	{
		MyTank.center = vec2(0.5f, 0.0f);
	}
}

TankBattle::~TankBattle()
{
	if (transfer_) 
	{
		delete transfer_;
	}

	for (auto bullet : Mybullets)
	{
		delete bullet;
	}	

	Mybullets.clear();

	for (auto bullet : Enemybullets)
	{
		delete bullet;
	}

	Enemybullets.clear();
}

void TankBattle::update()
{
	// move tank
	bool is_moving = false;

	if (isKeyPressed(GLFW_KEY_LEFT)) 
	{
		MyTank.moveLeft(getTimeStep());
		is_moving = true;
	}

	if (isKeyPressed(GLFW_KEY_RIGHT)) 
	{
		MyTank.moveRight(getTimeStep());
		is_moving = true;
	}

	if (isKeyPressed(GLFW_KEY_UP)) 
	{
		MyTank.moveUp(getTimeStep());
		is_moving = true;
	}

	if (isKeyPressed(GLFW_KEY_DOWN)) 
	{
		MyTank.moveDown(getTimeStep());
		is_moving = true;
	}

	// shoot a cannon ball
	if (isKeyPressedAndReleased(GLFW_KEY_SPACE))
	{
		Bullet* NewBullet = new Bullet;

		NewBullet->center = MyTank.center;
		NewBullet->center.x += 0.2f;
		NewBullet->center.y += 0.1f;

		if (isPlayer1)
		{
			NewBullet->velocity = vec2(2.0f, 0.0f);
		}
		else
		{
			NewBullet->velocity = vec2(-2.0f, 0.0f);
		}

		Mybullets.push_back(NewBullet);
	}

	for (auto bullet : Mybullets) {
		bullet->update(getTimeStep());
	}

	// remove bullets that go out of the screen
	Mybullets.erase(
		std::remove_if(Mybullets.begin(), Mybullets.end(), [](Bullet* bullet) {
			bool shouldDelete = bullet->center.x > 2.0f || bullet->center.x < -2.0f || bullet->center.y > 2.0f || bullet->center.y < -2.0f;
			
			if (shouldDelete) 
			{
				delete bullet;
			}

			return shouldDelete;
			}),

		Mybullets.end()
	);

	render();

	if (transfer_) 
	{
		handleNetworkCommunication();
	}
}

void TankBattle::render()
{
	// rendering
	Wall.draw();

	MyTank.draw();
	for (auto bullet : Mybullets)
	{
		bullet->draw();
	}

	if (transfer_)
	{
		EnemyTank.draw();
		for (auto bullet : Enemybullets)
		{
			bullet->draw();
		}
	}	
}

void TankBattle::handleNetworkCommunication()
{
	transfer_->sendTankAndBullets(MyTank, Mybullets);

	transfer_->receiveEnemyTankAndBullets(EnemyTank, Enemybullets);
}