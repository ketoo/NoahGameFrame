#include <iostream>
#include <sstream>
#include "NFRedisClient.h"
#include "NFRedisTester.h"
#include "NFRedisTester.h"

int main()
{
	NFRedisTester xRedisTester;
	xRedisTester.RunTester();

	while (1)
	{

		xRedisTester.Execute();
	}

	return 0;
}
