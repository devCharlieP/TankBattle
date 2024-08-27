#include "Transfer.h"
#include <iostream>

Transfer::Transfer(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : socket_(io_context)
{
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket_, endpoints);
}

// 서버로 자신의 탱크 위치를 전송
void Transfer::sendTankPosition(float tankx, float tanky)
{
    boost::asio::write(socket_, boost::asio::buffer(&tankx, sizeof(float)));
    boost::asio::write(socket_, boost::asio::buffer(&tanky, sizeof(float)));
}

// 서버로부터 상대방의 탱크 위치를 수신
void Transfer::receiveTankPosition(float& tankx, float& tanky)
{
    boost::asio::read(socket_, boost::asio::buffer(&tankx, sizeof(float)));
    boost::asio::read(socket_, boost::asio::buffer(&tanky, sizeof(float)));
}

// 서버로부터 연결 상태를 수신
void Transfer::receiveStatus(bool& status, bool& is1p)
{
    boost::asio::read(socket_, boost::asio::buffer(&status, sizeof(bool)));
    boost::asio::read(socket_, boost::asio::buffer(&is1p, sizeof(bool)));
}
