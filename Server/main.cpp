#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

void receiveTankData(tcp::socket& socket, float& tankx, float& tanky)
{
	boost::asio::read(socket, boost::asio::buffer(&tankx, sizeof(float)));
	boost::asio::read(socket, boost::asio::buffer(&tanky, sizeof(float)));
}

void sendTankData(tcp::socket& socket, float tankx, float tanky)
{
	boost::asio::write(socket, boost::asio::buffer(&tankx, sizeof(float)));
	boost::asio::write(socket, boost::asio::buffer(&tanky, sizeof(float)));
}

void receiveBulletData(tcp::socket& socket, float& bulletx, float& bullety)
{
	boost::asio::read(socket, boost::asio::buffer(&bulletx, sizeof(float)));
	boost::asio::read(socket, boost::asio::buffer(&bullety, sizeof(float)));
}

void sendBulletData(tcp::socket& socket, float bulletx, float bullety)
{
	boost::asio::write(socket, boost::asio::buffer(&bulletx, sizeof(float)));
	boost::asio::write(socket, boost::asio::buffer(&bullety, sizeof(float)));
}

void receiveBulletCount(tcp::socket& socket, int& count)
{
	boost::asio::read(socket, boost::asio::buffer(&count, sizeof(int)));
}

void sendBulletCount(tcp::socket& socket, int count)
{
	boost::asio::write(socket, boost::asio::buffer(&count, sizeof(int)));
}

void sendStatus(tcp::socket& socket, bool status, bool is1p)
{
	boost::asio::write(socket, boost::asio::buffer(&status, sizeof(bool)));
	boost::asio::write(socket, boost::asio::buffer(&is1p, sizeof(bool)));
}

int main()
{
	try
	{
		//init
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
		std::cout << "서버 실행중..." << std::endl;

		//플레이어 1 연결
		tcp::socket socket1(io_context);
		acceptor.accept(socket1);
		cout << "플레이어 1 연결됨" << endl;

		bool status = false;
		bool is1p = true;

		//플레이어 1에게 플레이어2 접속 상태(false)와 1p 전송
		sendStatus(socket1, status, is1p);

		//플레이어 2 연결
		tcp::socket socket2(io_context);
		acceptor.accept(socket2);
		cout << "플레이어 2 연결됨" << endl;

		//플레이어 1에게 플레이어2 접속 상태(true)와 1p 전송
		status = true;
		sendStatus(socket1, status, is1p);

		//플레이어 2에게 플레이어1 접속 상태(true)와 2p 전송
		is1p = false;
		sendStatus(socket2, status, is1p);

		float tankx, tanky;
		float bulletx, bullety;
		int bulletCount;

		while (1)
		{
			receiveTankData(socket1, tankx, tanky);
			sendTankData(socket2, tankx, tanky);

			receiveBulletCount(socket1, bulletCount);
			sendBulletCount(socket2, bulletCount);

			for (int i = 0; i < bulletCount; i++)
			{
				receiveBulletData(socket1, bulletx, bullety);
				sendBulletData(socket2, bulletx, bullety);
			}

			receiveTankData(socket2, tankx, tanky);
			sendTankData(socket1, tankx, tanky);

			receiveBulletCount(socket2, bulletCount);
			sendBulletCount(socket1, bulletCount);

			for (int i = 0; i < bulletCount; i++)
			{
				receiveBulletData(socket2, bulletx, bullety);
				sendBulletData(socket1, bulletx, bullety);
			}
		}
	}
	catch (exception& e)
	{
		cerr << "예외 발생: " << e.what() << endl;
	}

	return 0;
}