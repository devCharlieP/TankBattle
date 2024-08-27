#include "TankBattle.h"
#include <iostream>

TankBattle::TankBattle()
    : Game2D("This is my digital canvas!", 1920, 1080, true, 2),
    transfer(io_context, "192.168.45.144", "12345"), status(false), is1p(true)
{
    // �������� ������ ����
    std::thread([this]() {
        while (!status) 
        {
            // �����κ��� ���¸� ���������� ����
            transfer.receiveStatus(status, is1p); 

            if (!status) 
            {
                // ���°� true�� �� ������ ���
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            }
            handle1p();
        }
        }).detach(); // �񵿱������� ���� ���� ������ ����
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
        if (isMyTankVisible)
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
    }

    for (auto bullet : Mybullets) 
    {
        bullet->update(getTimeStep());
    }

    //ȭ�� �� źȯ �����
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

    handleTankCollision();
    
    handleTankVisibility();
}

void TankBattle::render()
{
    Wall.draw();

    if (isMyTankVisible)
    {
        MyTank.draw();
    }

    for (auto bullet : Mybullets)
    {
        bullet->draw();
    }

    if (isEnemyTankVisible)
    {
        EnemyTank.draw();
    }

    for (auto bullet : Enemybullets)
    {
        bullet->draw();
    }
}

void TankBattle::handletransfer()
{
    transfer.sendTankPosition(MyTank.center.x, MyTank.center.y);

    int bulletCount = Mybullets.size();
    transfer.sendBulletCount(bulletCount);

    for (auto bullet : Mybullets)
    {
        transfer.sendBulletPosition(bullet->center.x, bullet->center.y);
    }

    transfer.receiveTankPosition(EnemyTank.center.x, EnemyTank.center.y);

    int enemyBulletCount = transfer.receiveBulletCount();
    Enemybullets.clear();

    for (int i = 0; i < enemyBulletCount; i++)
    {
        float bulletX, bulletY;
        transfer.receiveBulletPosition(bulletX, bulletY);

        Bullet* NewEnemyBullet = new Bullet;
        NewEnemyBullet->center = vec2(bulletX, bulletY);
        Enemybullets.push_back(NewEnemyBullet);
    }
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

void TankBattle::handleTankCollision()
{
    vec2 distance;

    // ���� źȯ�� �� ��ũ�� �浹 �˻�
    for (auto bullet : Enemybullets)
    {
        distance = bullet->center - MyTank.center;

        if (abs(distance.x) < 0.1f && abs(distance.y) < 0.12f && isMyTankVisible)
        {
            isMyTankVisible = false;
            hitTime = std::chrono::steady_clock::now();
        }
    }

    // �� źȯ�� ���� ��ũ�� �浹 �˻�
    for (auto bullet : Mybullets)
    {
        distance = bullet->center - EnemyTank.center;

        if (abs(distance.x) < 0.1f && abs(distance.y) < 0.12f && isEnemyTankVisible)
        {
            isEnemyTankVisible = false;
            enemyHitTime = std::chrono::steady_clock::now();
        }
    }
}

void TankBattle::handleTankVisibility()
{
    auto now = std::chrono::steady_clock::now();

    // �� ��ũ�� ���� ������ �� 3�� ��� �� �ʱ� ��ġ�� ����
    if (!isMyTankVisible && std::chrono::duration_cast<std::chrono::seconds>(now - hitTime).count() >= 3)
    {
        MyTank.center = is1p ? vec2(-1.0f, 0.0f) : vec2(1.0f, 0.0f);
        isMyTankVisible = true;
    }

    // ��� ��ũ�� ���� ������ �� 3�� ��� �� �ʱ� ��ġ�� ����
    if (!isEnemyTankVisible && std::chrono::duration_cast<std::chrono::seconds>(now - enemyHitTime).count() >= 3)
    {
        EnemyTank.center = is1p ? vec2(1.0f, 0.0f) : vec2(-1.0f, 0.0f);
        isEnemyTankVisible = true;
    }
}