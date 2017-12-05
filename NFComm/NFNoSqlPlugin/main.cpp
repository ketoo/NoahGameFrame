#include <iostream>
#include <sstream>
#include "NFRedisClient.h"
#include "NFRedisTester.h"
#include "NFRedisTester.h"

void main()
{
	NFRedisTester xRedisTester;
	xRedisTester.RunTester();

	while (1)
	{

		xRedisTester.Execute();
	}

	return;
}