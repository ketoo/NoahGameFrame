#include <iostream>
#include <sstream>
#include "NFRedisClient.h"
#include "NFRedisTester.h"
#include "NFRedisTester.h"
using namespace std;

#pragma comment( lib, "ws2_32" )

#ifdef _DEBUG
#pragma comment( lib, "hiredis_d.lib") 
#else
#pragma comment( lib, "hiredis.lib") 
#endif

#define _IP_ "127.0.0.1"
#define _PORT_ 6379

int main()
{
	NFRedisTester xRedisTester(_IP_, _PORT_);
	cout << "test begin......" << endl;
	while (1)
	{
		if (xRedisTester.IsConnect())
		{
			if (xRedisTester.RunTester())
			{
				break;
			}
		}

		xRedisTester.Execute();
	}

	cout << "test end......" << endl;
	std::getchar();
	return 0;
}
