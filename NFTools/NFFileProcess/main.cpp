#include "FileProcess.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdint>
#include <vector>
#include <string>

int main(int argc, const char *argv[])
{
	for (int i = 0;i < 1;i++)
	{
		auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		FileProcess* fp = new FileProcess();
		fp->OnCreateXMLFile();
		fp->OnCreateMysqlFile();
		auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::cout << "Generate All Done for Good!!!" << std::endl;
		std::cout << "Timespan:" << t2 - t1 << "us" << std::endl;
		delete fp;
	}



	std::cin.ignore();
	return 0;
}
