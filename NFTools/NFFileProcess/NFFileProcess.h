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
#include <list>


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

class NFClassProperty
{
public:
	NFClassProperty()
	{
	}

	std::map<std::string, std::string> descList;//tag, value
	std::string name;
	std::string type;
};

class NFClassRecord
{
public:
	NFClassRecord()
	{
	}

	struct RecordColDesc
	{
		int index;
		std::string type;
		std::string desc;
	};
	
	std::string strClassName;
	std::map<std::string, std::string> descList;//tag, value
	std::map<std::string, RecordColDesc*> colList;//tag, desc
};

class NFClassStruct
{
public:
	NFClassStruct()
	{
	}
	std::string strClassName;
	std::map<std::string, NFClassProperty*> propertyList;//key, desc
	std::map<std::string, NFClassRecord*> xObjectRecordList;//name, desc
};

class NFClassIni
{
public:
	NFClassIni()
	{
	}

	class IniData
	{
	public:
		std::map<std::string, std::string> propertyList;
	};

	std::map<std::string, IniData*> iniList;//key, iniList
};

class ClassData
{
public:
	NFClassStruct xStructData;
	NFClassIni xIni;
};

class NFFileProcess
{
public:
	NFFileProcess();
	~NFFileProcess();

	///////////////////////////////////////////////////////////////

	bool LoadDataFromExcel();
	bool LoadDataFromExcel(const std::string& strFile, const std::string& strFileName);
	bool LoadDataFromExcel(MiniExcelReader::Sheet& sheet, ClassData* pClassData);

	bool LoadIniData(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessProperty(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessComponent(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessRecord(MiniExcelReader::Sheet& sheet, ClassData* pClassData);

	///////////////////////////////////////////////////////////////

	bool Save();
	bool SaveForCPP();
	bool SaveForCS();
	bool SaveForJAVA();
	bool SaveForPB();
	bool SaveForSQL();

	bool SaveForIni();

	///////////////////////////////////////////////////////////////
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
	const int nPropertyHeight = 10;//property line
	const int nRecordHeight = 11;//record line
	const int nRecordDescHeight = 7;//record line

	std::vector<char *> tmpStrList;
	std::string strCipherCfg = "conf";

	std::string strExecutePath = "NFDataCfg/";
	std::string strToolBasePath = "../";
	std::string strRelativePath = "../../";
	std::string strExcelStructPath = "Excel_Struct/";
	std::string strXMLStructPath = "Struct/";

	std::string strExcelIniPath = "Excel_Ini/";
	std::string strXMLIniPath = "Ini/";

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

	std::map<std::string, ClassData*> mxClassData;
};
