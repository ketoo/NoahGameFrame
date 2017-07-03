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

class NFClassStruct
{
public:
	NFClassStruct()
	{
		//xObject = NF_SHARE_PTR<NFIObject>(new NFCObject(NFGUID(), nullptr));
	}

	std::string strPath;
	FILE* xWriter;
	//NF_SHARE_PTR<NFIObject> xObject;
};

class NFFileProcess
{
public:
	NFFileProcess();
	~NFFileProcess();

	void OnCreateXMLFile();
	void OnCreateMysqlFile();
	void SetUTF8(const bool b);

private:
	void CreateProtoFile();
	void CreateFileHead();
	void CreateStructThreadFunc();
	void CreateIniThreadFunc();
	bool CreateStructXML(std::string strFile, std::string strFileName);
	bool CreateIniXML(std::string strFile);
	bool LoadLogicClass(std::string strFile);
	bool LoadClass(std::string strFile, std::string strTable);

	std::vector<std::string> GetFileListInFolder(std::string folderPath, int depth);
	void StringReplace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
	char * NewChar(const std::string& str);

private:

	//NFMapEx<std::string, NFClassStruct> mxClassStruct;

	bool bConvertIntoUTF8 = false;

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


};
