#pragma once
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "Dependencies/common/lexical_cast.hpp"
#include "MiniExcelReader.h"
#include "NFFileData.h"
#include "NFIGenerator.h"
#include "NFFileProcessHelp.hpp"

class NFFileProcess
{
public:
	NFFileProcess();
	virtual ~NFFileProcess();

	bool LoadDataFromExcel();

	void SetUTF8(const bool b);

	bool GenerateData();

	void PrintData();
	void PrintData(ClassData* data);

private:
	bool LoadDataFromExcel(const std::string& filePath, const std::string& fileName);
	bool LoadIncludeExcel(ClassData* pClassData, const std::string& strFile, const std::string& fileName);

	bool LoadDataFromExcel(MiniExcelReader::Sheet& sheet, ClassData* pClassData);

	bool LoadIniData(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessProperty(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessComponent(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessRecord(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessIncludes(MiniExcelReader::Sheet& sheet, ClassData* pClassData);
	bool LoadDataAndProcessRef(MiniExcelReader::Sheet& sheet, ClassData* pClassData);

	void ProcessParts();
	void ProcessIncludeFiles();
	void ProcessRefFiles();


private:

	bool bConvertIntoUTF8 = false;

	const int nPropertyHeight = 10;//property line
	const int nRecordHeight = 13;//record line
	const int nRecordDescHeight = 9;//record line

	std::string strExcelIniPath = "../Excel/";
	std::string strXMLStructPath = "../Struct/";
	std::string strXMLIniPath = "../Ini/";

	std::map<std::string, ClassData*> mxClassData;
	std::vector<NFIGenerator*> mxGenerators;
};
