#include "TankBattle.h"
#include <iostream>

TankBattle::TankBattle()
    : Game2D("This is my digital canvas!", 1920, 1080, true, 2),
    transfer(io_context, "127.0.0.1", "12345"), status(false), is1p(true)
{
    // 서버와의 연결을 설정
    std::thread([this]() {
        while (!status) 
        {
            transfer.receiveStatus(status, is1p); // 서버로부터 상태를 지속적으로 수신

            if (!status) 
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 상태가 true가 될 때까지 대기
            }

            handle1p();
        }
        }).detach(); // 비동기적으로 상태 수신 스레드 실행
}

TankBattle::~TankBattle()
{
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
    if (!status)
    {
        return;
    }

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

    if (isKeyPressedAndReleased(GLFW_KEY_SPACE)) 
    {
        Bullet* NewBullet = new Bullet;
        NewBullet->center = MyTank.center;

        if (is1p)
        {
            NewBullet->center.x += 0.2f;
            NewBullet->center.y += 0.1f;
            NewBullet->velocity = vec2(2.0f, 0.0f);
        }
        else
        {
            NewBullet->center.x -= 0.2f;
            NewBullet->center.y += 0.1f;
            NewBullet->velocity = vec2(-2.0f, 0.0f);
        }

        Mybullets.push_back(NewBullet);
    }

    for (auto bullet : Mybullets) 
    {
        bullet->update(getTimeStep());
    }

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

    handletransfer();
}

void TankBattle::render()
{
    Wall.draw();

    MyTank.draw();
    for (auto bullet : Mybullets) 
    {
        bullet->draw();
    }

    EnemyTank.draw();
    for (auto bullet : Enemybullets) 
    {
        bullet->draw();
    }
}

void TankBattle::handletransfer()
{
    transfer.sendTankPosition(MyTank.center.x, MyTank.center.y);
    
    transfer.receiveTankPosition(EnemyTank.center.x, EnemyTank.center.y);
}

void TankBattle::handle1p()
{
    if (is1p)
    {
        MyTank.center = vec2(-1.0f, 0.0f);
        MyTank.direction = 1.0f;
        EnemyTank.direction = -1.0f;
    }
    else
    {
        MyTank.center = vec2(1.0f, 0.0f);
        MyTank.direction = -1.0f;
        EnemyTank.direction = 1.0f;
    }
}