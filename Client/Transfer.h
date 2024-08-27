#include <boost/asio.hpp>

class Transfer
{
public:
    Transfer(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    void sendTankPosition(float tankx, float tanky);

    void receiveTankPosition(float& tankx, float& tanky);

    void sendBulletPosition(float bulletx, float bullety);

    void receiveBulletPosition(float &bulletx, float &bullety);

    void sendBulletCount(int count);

    int receiveBulletCount();

    void receiveStatus(bool& status, bool& is1p);

private:
    boost::asio::ip::tcp::socket socket_;
};
