#include <iostream>
#include <sstream>
#include "NFRedisClient.h"
#include "NFRedisTester.h"
#include "NFRedisTester.h"

#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "hiredis_d.lib") 

int main()
{
	NFRedisTester xRedisTester;
	xRedisTester.RunTester();

	std::cout << "test over" << std::endl;

	while (1)
	{
		xRedisTester.Execute();
	}

	return 0;
}
