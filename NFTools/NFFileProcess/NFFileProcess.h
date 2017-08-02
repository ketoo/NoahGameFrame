#pragma once
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "Dependencies/common/lexical_cast.hpp"
#include "MiniExcelReader.h"
#include <map>


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
	std::map<std::string, NFClassProperty*> xPropertyList;//key, desc
	std::map<std::string, NFClassRecord*> xRecordList;//name, desc
};

class NFClassElement
{
public:
	NFClassElement()
	{
	}

	class ElementData
	{
	public:
		std::map<std::string, std::string> xPropertyList;
	};

	std::map<std::string, ElementData*> xElementList;//key, iniList
};

class ClassData
{
public:
	NFClassStruct xStructData;
	NFClassElement xIniData;
};

class NFFileProcess
{
public:
	NFFileProcess();
	virtual ~NFFileProcess();

	bool LoadDataFromExcel();
	bool Save();

	void SetUTF8(const bool b);

private:
	bool LoadDataFromExcel(const std::string& strFile, const std::string& strFileName);
	bool LoadDataFromExcel(MiniExcelReader::Sheet& sheet, ClassData* pClassData);

	bool LoadIniData(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessProperty(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessComponent(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessRecord(MiniExcelReader::Sheet& sheet, ClassData* pClassData);

	bool SaveForCPP();
	bool SaveForCS();
	bool SaveForJAVA();
	bool SaveForPB();
	bool SaveForSQL();

	bool SaveForStruct();
	bool SaveForIni();

	bool SaveForLogicClass();


	std::vector<std::string> GetFileListInFolder(std::string folderPath, int depth);
	void StringReplace(std::string &strBig, const std::string &strsrc, const std::string &strdst);

private:

	bool bConvertIntoUTF8 = false;

	const int nPropertyHeight = 10;//property line
	const int nRecordHeight = 11;//record line
	const int nRecordDescHeight = 7;//record line

	std::string strExcelIniPath = "../Excel/";
	std::string strXMLStructPath = "../Struct/";
	std::string strXMLIniPath = "../Ini/";

	std::string strMySQLFile = "../mysql/NFrame.sql";
	std::string strProtoFile = "../proto/NFRecordDefine.proto";
	std::string strHPPFile = "../proto/NFProtocolDefine.hpp";
	std::string strCPPFile = "../proto/NFProtocolDefine.cpp";
	std::string strJavaFile = "../proto/NFProtocolDefine.java";
	std::string strCSFile = "../proto/NFProtocolDefine.cs";

	std::map<std::string, ClassData*> mxClassData;
};
