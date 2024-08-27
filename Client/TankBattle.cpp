#include "TankBattle.h"
#include <iostream>

TankBattle::TankBattle()
    : Game2D("This is my digital canvas!", 1920, 1080, true, 2),
    transfer(io_context, "192.168.45.144", "12345"), status(false), is1p(true)
{
    // 서버와의 연결을 설정
    std::thread([this]() {
        while (!status) 
        {
            // 서버로부터 상태를 지속적으로 수신
            transfer.receiveStatus(status, is1p); 

            if (!status) 
            {
                // 상태가 true가 될 때까지 대기
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
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

    //화면 밖 탄환 지우기
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

    // 상대방 탄환과 내 탱크의 충돌 검사
    for (auto bullet : Enemybullets)
    {
        distance = bullet->center - MyTank.center;

        if (abs(distance.x) < 0.1f && abs(distance.y) < 0.12f && isMyTankVisible)
        {
            isMyTankVisible = false;
            hitTime = std::chrono::steady_clock::now();
        }
    }

    // 내 탄환과 상대방 탱크의 충돌 검사
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

    // 내 탱크가 투명 상태일 때 3초 경과 후 초기 위치로 복귀
    if (!isMyTankVisible && std::chrono::duration_cast<std::chrono::seconds>(now - hitTime).count() >= 3)
    {
        MyTank.center = is1p ? vec2(-1.0f, 0.0f) : vec2(1.0f, 0.0f);
        isMyTankVisible = true;
    }

    // 상대 탱크가 투명 상태일 때 3초 경과 후 초기 위치로 복귀
    if (!isEnemyTankVisible && std::chrono::duration_cast<std::chrono::seconds>(now - enemyHitTime).count() >= 3)
    {
        EnemyTank.center = is1p ? vec2(1.0f, 0.0f) : vec2(-1.0f, 0.0f);
        isEnemyTankVisible = true;
    }
}