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
		std::cout << "���� ������..." << std::endl;

		//�÷��̾� 1 ����
		tcp::socket socket1(io_context);
		acceptor.accept(socket1);
		cout << "�÷��̾� 1 �����" << endl;

		bool status = false;
		bool is1p = true;

		//�÷��̾� 1���� �÷��̾�2 ���� ����(false)�� 1p ����
		sendStatus(socket1, status, is1p);

		//�÷��̾� 2 ����
		tcp::socket socket2(io_context);
		acceptor.accept(socket2);
		cout << "�÷��̾� 2 �����" << endl;

		//�÷��̾� 1���� �÷��̾�2 ���� ����(true)�� 1p ����
		status = true;
		sendStatus(socket1, status, is1p);

		//�÷��̾� 2���� �÷��̾�1 ���� ����(true)�� 2p ����
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
		cerr << "���� �߻�: " << e.what() << endl;
	}

	return 0;
}