#include <iostream>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void receiveData(tcp::socket& socket, int& tankCenter, int& bulletCount, std::vector<int>& bulletCenters)
{
    boost::asio::read(socket, boost::asio::buffer(&tankCenter, sizeof(int)));

    boost::asio::read(socket, boost::asio::buffer(&bulletCount, sizeof(int)));

    bulletCenters.resize(bulletCount);
    boost::asio::read(socket, boost::asio::buffer(bulletCenters.data(), bulletCount * sizeof(int)));
}

void sendData(tcp::socket& socket, int tankCenter, int bulletCount, const std::vector<int>& bulletCenters)
{
    boost::asio::write(socket, boost::asio::buffer(&tankCenter, sizeof(int)));

    boost::asio::write(socket, boost::asio::buffer(&bulletCount, sizeof(int)));

    boost::asio::write(socket, boost::asio::buffer(bulletCenters.data(), bulletCount * sizeof(int)));
}

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));

        std::cout << "Server is running, waiting for players..." << std::endl;

        tcp::socket socket1(io_context);
        tcp::socket socket2(io_context);

        acceptor.accept(socket1);
        std::cout << "Player 1 connected." << std::endl;

        int player1 = 1;
        boost::asio::write(socket1, boost::asio::buffer(&player1, sizeof(int)));

        acceptor.accept(socket2);
        std::cout << "Player 2 connected." << std::endl;

        int player2 = 2;
        boost::asio::write(socket2, boost::asio::buffer(&player2, sizeof(int)));

        int tankCenter1 = 0, tankCenter2 = 0;
        int bulletCount1 = 0, bulletCount2 = 0;
        std::vector<int> bulletCenters1, bulletCenters2;

        bool player1_connected = true;
        bool player2_connected = true;

        while (player1_connected || player2_connected)
        {
            if (player1_connected)
            {
                try
                {
                    receiveData(socket1, tankCenter2, bulletCount2, bulletCenters2);
                    sendData(socket2, tankCenter2, bulletCount2, bulletCenters2);
                }
                catch (std::exception& e)
                {
                    std::cerr << "Player 1 disconnected: " << e.what() << std::endl;
                    player1_connected = false;
                }
            }

            if (player2_connected)
            {
                try
                {
                    receiveData(socket2, tankCenter1, bulletCount1, bulletCenters1);
                    sendData(socket1, tankCenter1, bulletCount1, bulletCenters1);
                }
                catch (std::exception& e)
                {
                    std::cerr << "Player 2 disconnected: " << e.what() << std::endl;
                    player2_connected = false;
                }
            }
        }

        std::cout << "Both players disconnected. Server shutting down." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
