#include <boost/asio.hpp>

class Transfer
{
public:
    Transfer(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    // 서버로 자신의 탱크 위치를 전송
    void sendTankPosition(float tankx, float tanky);

    // 서버로부터 상대방의 탱크 위치를 수신
    void receiveTankPosition(float& tankx, float& tanky);

    // 서버로부터 연결 상태를 수신
    void receiveStatus(bool& status, bool& is1p);

private:
    boost::asio::ip::tcp::socket socket_;
};
