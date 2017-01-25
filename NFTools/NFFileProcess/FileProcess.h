#pragma once
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "Dependencies/common/lexical_cast.hpp"
//#include "tinyxml2.h"
#include "MiniExcelReader.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <map>


#if NF_PLATFORM == NF_PLATFORM_WIN
#include <io.h>
#include <windows.h>
#include <conio.h>
#else
#include <iconv.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#endif



class FileProcess
{
public:
	FileProcess();
	~FileProcess();

	void CreateProtoFile();
	void CreateNameFile();
	void OnCreateXMLFile();
	void CreateStructThreadFunc();
	void CreateIniThreadFunc();
	bool CreateStructXML(std::string strFile, std::string strFileName);
	bool CreateIniXML(std::string strFile);
	void OnCreateMysqlFile();
	bool LoadLogicClass(std::string strFile);
	bool LoadClass(std::string strFile, std::string strTable);

	bool bConvertIntoUTF8 = false;

private:
	char * NewChar(const std::string& str);
	int nCipher = 0;
	std::vector<char *> tmpStrList;
	std::string strCipherCfg = "conf";

	std::string strExecutePath = "NFDataCfg/";
	std::string strToolBasePath = "../";
	std::string strRelativePath = "../../";
	std::string strExcelStructPath = "Excel_Struct/";
	std::string strXMLStructPath = "Struct/Class/";

	std::string strExcelIniPath = "Excel_Ini/";
	std::string strXMLIniPath = "Ini/NPC/";

	std::string strLogicClassFile = "";
	std::string strMySQLFile = "../mysql/NFrame.sql";
	std::string strMySQLClassFile = "../mysql/NFClass.sql";

	std::string strProtoFile = "../proto/NFRecordDefine.proto";

	std::string strHPPFile = "../proto/NFProtocolDefine.hpp";
	std::string strJavaFile = "../proto/NFProtocolDefine.java";
	std::string strCSFile = "../proto/NFProtocolDefine.cs";

	FILE* mysqlWriter = nullptr;
	FILE* mysqlClassWriter = nullptr;
	FILE* protoWriter = nullptr;
	FILE* hppWriter = nullptr;
	FILE* javaWriter = nullptr;
	FILE* csWriter = nullptr;

	std::string strHppIObjectInfo;
	std::string strJavaIObjectInfo;
	std::string strCSIObjectInfo;

	int nRecordStart = 11;

	std::vector<std::string> GetFileListInFolder(std::string folderPath, int depth)
	{
		std::vector<std::string> result;
#if NF_PLATFORM == NF_PLATFORM_WIN
		_finddata_t FileInfo;
		std::string strfind = folderPath + "\\*";
		long long Handle = _findfirst(strfind.c_str(), &FileInfo);


		if (Handle == -1L)
		{
			std::cerr << "can not match the folder path" << std::endl;
			exit(-1);
		}
		do {
			
			if (FileInfo.attrib & _A_SUBDIR)
			{
				
				if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
				{
					std::string newPath = folderPath + "\\" + FileInfo.name;
					//dfsFolder(newPath, depth);
				}
			}
			else
			{

				std::string filename = (folderPath + "\\" + FileInfo.name);
				result.push_back(filename);
			}
		} while (_findnext(Handle, &FileInfo) == 0);


		_findclose(Handle);
#else
	DIR *pDir;
	struct dirent *ent;
	char childpath[512];
	char absolutepath[512];
	pDir = opendir(folderPath.c_str());
	memset(childpath, 0, sizeof(childpath));
	while ((ent = readdir(pDir)) != NULL)
	{
		if (ent->d_type & DT_DIR)
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			{
				continue;
			}
		}
		else
		{
			sprintf(absolutepath, "%s/%s", folderPath.c_str(), ent->d_name);
			result.push_back(absolutepath);
		}
	}

	sort(result.begin(), result.end());
#endif
		return result;
	}

	void StringReplace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
	{
		std::string::size_type pos = 0;
		std::string::size_type srclen = strsrc.size();
		std::string::size_type dstlen = strdst.size();

		while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
		{
			strBig.replace(pos, srclen, strdst);
			pos += dstlen;
		}
	}
};
