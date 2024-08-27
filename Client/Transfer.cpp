#include "Transfer.h"

Transfer::Transfer(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : socket_(io_context)
{
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket_, endpoints);
}

void Transfer::sendTankPosition(float tankx, float tanky)
{
    boost::asio::write(socket_, boost::asio::buffer(&tankx, sizeof(float)));
    boost::asio::write(socket_, boost::asio::buffer(&tanky, sizeof(float)));
}

void Transfer::receiveTankPosition(float& tankx, float& tanky)
{
    boost::asio::read(socket_, boost::asio::buffer(&tankx, sizeof(float)));
    boost::asio::read(socket_, boost::asio::buffer(&tanky, sizeof(float)));
}

void Transfer::sendBulletPosition(float bulletx, float bullety)
{
    boost::asio::write(socket_, boost::asio::buffer(&bulletx, sizeof(float)));
    boost::asio::write(socket_, boost::asio::buffer(&bullety, sizeof(float)));
}

void Transfer::receiveBulletPosition(float& bulletx, float& bullety)
{
    boost::asio::read(socket_, boost::asio::buffer(&bulletx, sizeof(float)));
    boost::asio::read(socket_, boost::asio::buffer(&bullety, sizeof(float)));
}

void Transfer::sendBulletCount(int count)
{
    boost::asio::write(socket_, boost::asio::buffer(&count, sizeof(int)));
}

int Transfer::receiveBulletCount()
{
    int count;
    boost::asio::read(socket_, boost::asio::buffer(&count, sizeof(int)));
    return count;
}

void Transfer::receiveStatus(bool& status, bool& is1p)
{
    boost::asio::read(socket_, boost::asio::buffer(&status, sizeof(bool)));
    boost::asio::read(socket_, boost::asio::buffer(&is1p, sizeof(bool)));
}
