#include "NFFileProcess.h"
#include <iostream>
#include <chrono>

#include "NFComm/NFPluginModule/NFPlatform.h"

int main(int argc, const char *argv[])
{
	auto t1 = NFGetTimeMS();

	NFFileProcess fp;
	fp.SetUTF8(false);//set it true to convert UTF8 to GBK which is to show Chinese words in NF
	fp.LoadDataFromExcel();

	fp.PrintData();

	fp.GenerateData();


	auto t2 = NFGetTimeMS();
	std::cout << "All Done!!!" << std::endl;
	std::cout << "Timespan: "  << (t2 - t1) << " ms" << std::endl;

	return 0;
}
