#pragma once

#include "TankBattle.h"
#include <boost/asio.hpp>

class Transfer {
public:
    Transfer(boost::asio::ip::tcp::socket& socket);

    bool receivePlayerInfo();

    void sendTankAndBullets(const Tank& tank, const std::vector<Bullet*>& bullets);

    void receiveEnemyTankAndBullets(Tank& enemyTank, std::vector<Bullet*>& enemyBullets);

private:
    boost::asio::ip::tcp::socket& socket_;
};
