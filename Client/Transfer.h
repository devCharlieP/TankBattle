#include <boost/asio.hpp>

class Transfer
{
public:
    Transfer(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    // ������ �ڽ��� ��ũ ��ġ�� ����
    void sendTankPosition(float tankx, float tanky);

    // �����κ��� ������ ��ũ ��ġ�� ����
    void receiveTankPosition(float& tankx, float& tanky);

    // �����κ��� ���� ���¸� ����
    void receiveStatus(bool& status, bool& is1p);

private:
    boost::asio::ip::tcp::socket socket_;
};
