#include "Transfer.h"
#include <thread>

Transfer::Transfer(boost::asio::ip::tcp::socket& socket)
    : socket_(socket)
{

}

bool Transfer::receivePlayerInfo()
{
    int playerNumber;
    boost::asio::read(socket_, boost::asio::buffer(&playerNumber, sizeof(int)));

    return playerNumber == 1;
}

void Transfer::sendTankAndBullets(const Tank& tank, const std::vector<Bullet*>& bullets)
{
    boost::asio::write(socket_, boost::asio::buffer(&tank.center, sizeof(vec2)));

    int bulletCount = bullets.size();
    boost::asio::write(socket_, boost::asio::buffer(&bulletCount, sizeof(int)));

    for (auto bullet : bullets) 
    {
        boost::asio::write(socket_, boost::asio::buffer(&bullet->center, sizeof(vec2)));
    }
}

void Transfer::receiveEnemyTankAndBullets(Tank& enemyTank, std::vector<Bullet*>& enemyBullets)
{
    boost::asio::read(socket_, boost::asio::buffer(&enemyTank.center, sizeof(vec2)));

    int bulletCount;
    boost::asio::read(socket_, boost::asio::buffer(&bulletCount, sizeof(int)));

    for (auto bullet : enemyBullets) 
    {
        delete bullet;
    }
    enemyBullets.clear();

    for (int i = 0; i < bulletCount; ++i) 
    {
        Bullet* newBullet = new Bullet;
        boost::asio::read(socket_, boost::asio::buffer(&newBullet->center, sizeof(vec2)));
        enemyBullets.push_back(newBullet);
    }
}
