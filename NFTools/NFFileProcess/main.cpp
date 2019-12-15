#include "NFFileProcess.h"
#include <iostream>
#include <chrono>

#include "NFComm/NFPluginModule/NFPlatform.h"

int main(int argc, const char *argv[])
{
	int arg = 0;
	if (argc > 1)
	{
		arg = atoi(argv[1]);
	}
	for (int i = 0;i < 1;i++)
	{
		auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		NFFileProcess fp;
		fp.SetUTF8(false);//set it true to convert UTF8 to GBK for supporting chinese in NF to show. 
		fp.LoadDataFromExcel();
		fp.Save();
						   //fp->OnCreateXMLFile();
		//fp->OnCreateMysqlFile();
		auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::cout << "Generate All Done for Good!!!" << std::endl;
		std::cout << "Timespan: " << t2 - t1 << " us (" << (t2 - t1) / 1000000.0f << "s)" << std::endl;
	}

	return 0;
}
