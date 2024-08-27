#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

void receiveTankData(tcp::socket &socket, float &tankx, float &tanky)
{
	boost::asio::read(socket, boost::asio::buffer(&tankx, sizeof(float)));
	boost::asio::read(socket, boost::asio::buffer(&tanky, sizeof(float)));
}

void sendTankData(tcp::socket& socket, float tankx, float tanky)
{
	boost::asio::write(socket, boost::asio::buffer(&tankx, sizeof(float)));
	boost::asio::write(socket, boost::asio::buffer(&tanky, sizeof(float)));
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
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
		std::cout << "서버 실행중..." << std::endl;

		tcp::socket socket1(io_context);
		acceptor.accept(socket1);
		cout << "플레이어 1 연결됨" << endl;

		bool status = false;
		bool is1p = true;

		sendStatus(socket1, status, is1p);

		tcp::socket socket2(io_context);
		acceptor.accept(socket2);
		cout << "플레이어 2 연결됨" << endl;

		status = true;

		sendStatus(socket1, status, is1p);
		
		is1p = false;
		sendStatus(socket2, status, is1p);

		float tankx = 0, tanky = 0;

		while (1)
		{
			receiveTankData(socket1, tankx, tanky);
			sendTankData(socket2, tankx, tanky);

			receiveTankData(socket2, tankx, tanky);
			sendTankData(socket1, tankx, tanky);
		}
	}
	catch(exception& e)
	{
		cerr << "예외 발생: " << e.what() << endl;
	}

	return 0;
}