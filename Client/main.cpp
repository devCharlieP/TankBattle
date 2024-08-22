#include "TankBattle.h"
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <boost/asio.hpp>

using namespace std;

int main()
{
	TankBattle().run();

	return 0;
}