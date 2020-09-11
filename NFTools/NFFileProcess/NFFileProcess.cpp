#include "NFFileProcess.h"
#include "Utf8ToGbk.h"
#include <iostream>

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <io.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#else
#include <iconv.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif


NFFileProcess::NFFileProcess()
{

}

NFFileProcess::~NFFileProcess()
{

}

bool NFFileProcess::LoadDataFromExcel()
{
	LoadDataFromExcel("../Excel/Comm/IObject.xlsx", "IObject");

	auto fileList = GetFileListInFolder(strExcelIniPath, 1);

	///////////////////////////////////

	for (auto filePath : fileList)
	{
		StringReplace(filePath, "\\", "/");
		StringReplace(filePath, "//", "/");

		if ((int)(filePath.find("$")) != -1)
		{
			continue;
		}

		auto strExt = GetFileNameExtByPath(filePath);
		if (strExt != ".xlsx")
		{
			continue;
		}

		auto fileName = GetFileNameByPath(filePath);
		if (fileName == "IObject")
		{
			continue;
		}

		if (!LoadDataFromExcel(filePath, fileName))
		{
			std::cout << "Create " + fileName + " failed!" << std::endl;
			continue;
		}
	}

	//auto sideFolderList = GetFolderListInFolder(strExcelIniPath);
	
	auto dataList = mxClassData;

	//process part
	for (auto classData : mxClassData)
	{
		for (auto className : classData.second->parts)
		{
			ClassData* nowClassData = classData.second;
			ClassData* partialClass = dataList[className];
			if (partialClass)
			{
				partialClass->bePartialed = true;
				for (auto property : partialClass->xStructData.xPropertyList)
				{
					nowClassData->xStructData.xPropertyList[property.first] = property.second;
				}

				for (auto record : partialClass->xStructData.xRecordList)
				{
					nowClassData->xStructData.xRecordList[record.first] = record.second;
				}

				for (auto element : partialClass->xIniData.xElementList)
				{
					nowClassData->xIniData.xElementList[element.first] = element.second;
				}

				//partial class doesnt need to generate Ini xml file
				partialClass->xIniData.xElementList.clear();
			}
		}
	}

	//process include
	for (auto classData : mxClassData)
	{
		for (auto className : classData.second->includes)
		{
			ClassData* nowClassData = classData.second;
			ClassData* includeClassData = dataList[className];

			if (includeClassData)
			{
				includeClassData->beIncluded = true;

				for (auto property : includeClassData->xStructData.xPropertyList)
				{
					//nowClassData->xStructData.xPropertyList.insert(std::make_pair(property.first, property.second));

				}

				for (auto record : includeClassData->xStructData.xRecordList)
				{
					//nowClassData->xStructData.xRecordList.insert(std::make_pair(record.first, record.second));
				}
			}
		}
	}

	return true;
}

bool NFFileProcess::LoadDataFromExcel(const std::string & filePath, const std::string & fileName)
{
	if (mxClassData.find(fileName) != mxClassData.end())
	{
		std::cout << filePath << " exist!!!" << std::endl;
		return false;
	}

	std::cout << filePath << std::endl;

	ClassData* pClassData = new ClassData();
	mxClassData[fileName] = pClassData;
	pClassData->xStructData.className = fileName;

	pClassData->filePath = filePath;
	StringReplace(pClassData->filePath, GetFileNameExtByPath(filePath), "");

	pClassData->fileFolder = pClassData->filePath;
	StringReplace(pClassData->fileFolder, fileName, "");
	///////////////////////////////////////
	//load

	MiniExcelReader::ExcelFile* xExcel = new MiniExcelReader::ExcelFile();
	if (!xExcel->open(filePath.c_str()))
	{
		std::cout << "can't open" << filePath << std::endl;
		return false;
	}

	std::vector<MiniExcelReader::Sheet>& sheets = xExcel->sheets();
	for (MiniExcelReader::Sheet& sh : sheets)
	{
		LoadDataFromExcel(sh, pClassData);
	}

	//load parts

	auto fileList = GetFileListInFolder(pClassData->filePath, 0);
	for (auto filePath : fileList)
	{
		StringReplace(filePath, "\\", "/");
		StringReplace(filePath, "//", "/");

		std::string fileName = GetFileNameByPath(filePath);
		if (!fileName.empty())
		{
			std::string fileName = fileName;
			transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);

			if (fileName == "include")
			{
				LoadIncludeExcel(pClassData, filePath, fileName);
			}
			else
			{
				pClassData->parts.push_back(fileName);
			}
		}
	}


	return true;
}

bool NFFileProcess::LoadIncludeExcel(ClassData* pClassData, const std::string& strFile, const std::string& fileName)
{
	MiniExcelReader::ExcelFile* xExcel = new MiniExcelReader::ExcelFile();
	if (!xExcel->open(strFile.c_str()))
	{
		std::cout << "can't open" << strFile << std::endl;
		return false;
	}

	std::vector<MiniExcelReader::Sheet>& sheets = xExcel->sheets();
	for (MiniExcelReader::Sheet& sh : sheets)
	{
		LoadDataFromExcel(sh, pClassData);
	}

	return true;
}

bool NFFileProcess::LoadDataFromExcel(MiniExcelReader::Sheet & sheet, ClassData * pClassData)
{
	const MiniExcelReader::Range& dim = sheet.getDimension();

	std::string strSheetName = sheet.getName();
	transform(strSheetName.begin(), strSheetName.end(), strSheetName.begin(), ::tolower);

	if (strSheetName.find("property") != std::string::npos)
	{
		LoadDataAndProcessProperty(sheet, pClassData);
		LoadIniData(sheet, pClassData);
	}
	else if (strSheetName.find("component") != std::string::npos)
	{
		LoadDataAndProcessComponent(sheet, pClassData);
	}
	else if (strSheetName.find("record") != std::string::npos)
	{
		LoadDataAndProcessRecord(sheet, pClassData);
	}
	else if (strSheetName.find("include") != std::string::npos)
	{
		LoadDataAndProcessIncludes(sheet, pClassData);
	}
	else
	{
		std::cout << pClassData->xStructData.className << " " << strSheetName << std::endl;
		assert(0);
	}

	return true;
}

bool NFFileProcess::LoadIniData(MiniExcelReader::Sheet & sheet, ClassData * pClassData)
{
	const MiniExcelReader::Range& dim = sheet.getDimension();
	std::string strSheetName = sheet.getName();
	transform(strSheetName.begin(), strSheetName.end(), strSheetName.begin(), ::tolower);

	std::map<std::string, int> PropertyIndex;//col index
	for (int c = dim.firstCol + 1; c <= dim.lastCol; c++)
	{
		MiniExcelReader::Cell* cell = sheet.getCell(dim.firstRow, c);
		if (cell)
		{
			PropertyIndex[cell->value] = c;
		}
	}
	////////////
	for (int r = dim.firstRow + nPropertyHeight; r <= dim.lastRow; r++)
	{
		MiniExcelReader::Cell* pIDCell = sheet.getCell(r, dim.firstCol);
		if (pIDCell && !pIDCell->value.empty())
		{
			NFClassElement::ElementData* pIniObject = new NFClassElement::ElementData();
			pClassData->xIniData.xElementList[pIDCell->value] = pIniObject;

			for (std::map<std::string, int>::iterator itProperty = PropertyIndex.begin(); itProperty != PropertyIndex.end(); ++itProperty)
			{
				std::string propertyName = itProperty->first;
				int col = itProperty->second;

				MiniExcelReader::Cell* cell = sheet.getCell(r, col);
				if (cell)
				{
					pIniObject->xPropertyList[propertyName] = cell->value;
				}
				else
				{
					pIniObject->xPropertyList[propertyName] = "";
				}
			}
		}
	}
	
	return false;
}

bool NFFileProcess::LoadDataAndProcessProperty(MiniExcelReader::Sheet & sheet, ClassData * pClassData)
{
	const MiniExcelReader::Range& dim = sheet.getDimension();
	std::string strSheetName = sheet.getName();
	transform(strSheetName.begin(), strSheetName.end(), strSheetName.begin(), ::tolower);

	std::map<std::string, int> descIndex;//row index
	std::map<std::string, int> PropertyIndex;//col index
	for (int r = dim.firstRow + 1; r <= dim.firstRow + nPropertyHeight - 1; r++)
	{
		MiniExcelReader::Cell* cell = sheet.getCell(r, dim.firstCol);
		if (cell)
		{
			descIndex[cell->value] = r;
		}
	}

	for (int c = dim.firstCol + 1; c <= dim.lastCol; c++)
	{
		MiniExcelReader::Cell* cell = sheet.getCell(dim.firstRow, c);
		if (cell)
		{
			PropertyIndex[cell->value] = c;
		}
	}
	////////////
	
	for (std::map<std::string, int>::iterator itProperty = PropertyIndex.begin(); itProperty != PropertyIndex.end(); ++itProperty)
	{
		std::string propertyName = itProperty->first;
		int col = itProperty->second;

		////////////
		NFClassProperty* pClassProperty = new NFClassProperty();
		pClassData->xStructData.xPropertyList[propertyName] = pClassProperty;

		////////////
		for (std::map<std::string, int>::iterator itDesc = descIndex.begin(); itDesc != descIndex.end(); ++itDesc)
		{
			std::string descName = itDesc->first;
			int row = itDesc->second;

			
			MiniExcelReader::Cell* pCell = sheet.getCell(row, col);
			if (pCell)
			{
				std::string descValue = pCell->value;

				pClassProperty->descList[descName] = descValue;
			}
		}

	}

	return false;
}

bool NFFileProcess::LoadDataAndProcessComponent(MiniExcelReader::Sheet & sheet, ClassData * pClassData)
{
	const MiniExcelReader::Range& dim = sheet.getDimension();
	std::string strSheetName = sheet.getName();
	transform(strSheetName.begin(), strSheetName.end(), strSheetName.begin(), ::tolower);

	std::vector<std::string> colNames;
	for (int c = dim.firstCol; c <= dim.lastCol; c++)
	{
		MiniExcelReader::Cell* cell = sheet.getCell(dim.firstRow, c);
		if (cell)
		{
			colNames.push_back(cell->value);
		}
	}
	for (int r = dim.firstRow + 1; r <= dim.lastRow; r++)
	{
		std::string testValue = "";
		MiniExcelReader::Cell* cell = sheet.getCell(r, dim.firstCol);
		if (cell)
		{
			testValue = cell->value;
		}
		if (testValue == "")
		{
			continue;
		}
		//auto componentNode = structDoc.allocate_node(rapidxml::node_element, "Component", NULL);
		//componentNodes->append_node(componentNode);
		std::string strType = "";
		for (int c = dim.firstCol; c <= dim.lastCol; c++)
		{
			std::string name = colNames[c - 1];
			std::string value = "";
			MiniExcelReader::Cell* cell = sheet.getCell(r, c);
			if (cell)
			{
				std::string valueCell = cell->value;
				transform(valueCell.begin(), valueCell.end(), valueCell.begin(), ::toupper);
				if (valueCell == "TRUE" || valueCell == "FALSE")
				{
					value = valueCell == "TRUE" ? "1" : "0";
				}
				else
				{
					value = cell->value;
				}

				if (name == "Type")
				{
					strType = value;
				}

			}
			//componentNode->append_attribute(structDoc.allocate_attribute(NewChar(name), NewChar(value)));
		}
	}
	return false;
}

bool NFFileProcess::LoadDataAndProcessRecord(MiniExcelReader::Sheet & sheet, ClassData * pClassData)
{
	const MiniExcelReader::Range& dim = sheet.getDimension();
	std::string strSheetName = sheet.getName();
	transform(strSheetName.begin(), strSheetName.end(), strSheetName.begin(), ::tolower);

	for (int nIndex = 0; nIndex < 100; nIndex++)
	{
		int nStartRow = nIndex * nRecordHeight + 1;
		int nEndRow = (nIndex + 1) * nRecordHeight;

		MiniExcelReader::Cell* pTestCell = sheet.getCell(nStartRow, dim.firstCol);
		if (pTestCell)
		{
			MiniExcelReader::Cell* pNameCell = sheet.getCell(nStartRow, dim.firstCol + 1);
			std::string recordName = pNameCell->value;
			
			////////////

			NFClassRecord* pClassRecord = new NFClassRecord();
			pClassData->xStructData.xRecordList[recordName] = pClassRecord;
			////////////

			for (int r = nStartRow + 1; r <= nStartRow + nRecordDescHeight; r++)
			{
				MiniExcelReader::Cell* cellDesc = sheet.getCell(r, dim.firstCol);
				MiniExcelReader::Cell* cellValue = sheet.getCell(r, dim.firstCol + 1);

				pClassRecord->descList[cellDesc->value] = cellValue->value;
			}

			int nRecordCol = atoi(pClassRecord->descList["Col"].c_str());
			for (int c = dim.firstCol; c <= nRecordCol; c++)
			{
				int r = nStartRow + nRecordDescHeight + 1;
				MiniExcelReader::Cell* pCellColName = sheet.getCell(r, c);
				MiniExcelReader::Cell* pCellColType = sheet.getCell(r + 1, c);
				MiniExcelReader::Cell* pCellColDesc = sheet.getCell(r + 2, c);

				NFClassRecord::RecordColDesc* pRecordColDesc = new NFClassRecord::RecordColDesc();
				pRecordColDesc->index = c - 1;
				pRecordColDesc->type = pCellColType->value;
				if (pCellColDesc)
				{
					pRecordColDesc->desc = pCellColDesc->value;
				}

				pClassRecord->colList[pCellColName->value] = pRecordColDesc;
			}
		}
	}

	
	return true;
}

bool NFFileProcess::LoadDataAndProcessIncludes(MiniExcelReader::Sheet & sheet, ClassData * pClassData)
{
	const MiniExcelReader::Range& dim = sheet.getDimension();
	std::string strSheetName = sheet.getName();
	transform(strSheetName.begin(), strSheetName.end(), strSheetName.begin(), ::tolower);

	for (int r = dim.firstRow; r <= dim.lastRow; r++)
	{
		MiniExcelReader::Cell* cell = sheet.getCell(r, dim.firstCol);
		if (cell)
		{
			std::string valueCell = cell->value;
			bool included = false;
			for (std::string fileName : pClassData->includes)
			{
				if (valueCell == fileName)
				{
					included = true;
					break;
				}
			}

			if (!included)
			{
				pClassData->includes.push_back(valueCell);
			}
		}
	}


	return true;
}


bool NFFileProcess::Save()
{
	SaveForCPP();
	SaveForCS();
	SaveForTS();
	SaveForJAVA();


	SaveForPB();
	SaveForSQL();

	SaveForStruct();
	SaveForIni();

	SaveForLogicClass();

	return false;
}

bool NFFileProcess::SaveForCPP()
{
	FILE* hppWriter = fopen(strHPPFile.c_str(), "w");

	std::string strFileHead;

	strFileHead = strFileHead
	+ "// -------------------------------------------------------------------------\n"
	+ "//    @FileName         :    NFProtocolDefine.hpp\n"
	+ "//    @Author           :    NFrame Studio\n"
	+ "//    @Module           :    NFProtocolDefine\n"
	+ "// -------------------------------------------------------------------------\n\n"
	+ "#ifndef NF_PR_NAME_HPP\n"
	+ "#define NF_PR_NAME_HPP\n\n"
	+ "#include <string>\n"
	+ "namespace NFrame\n{\n";

	fwrite(strFileHead.c_str(), strFileHead.length(), 1, hppWriter);
	/////////////////////////////////////////////////////

	ClassData* pBaseObject = mxClassData["IObject"];
	std::string instanceField = "\n";

	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		if (it->second->bePartialed)
		{
			continue;
		}

		const std::string& className = it->first;
		ClassData* pClassDta = it->second;
		// cpp
		std::string strPropertyInfo;
		
		strPropertyInfo += "\tclass " + className + "\n\t{\n\tpublic:\n";
		strPropertyInfo += "\t\t//Class name\n\t";
		strPropertyInfo += "\tstatic const std::string& ThisName(){ static std::string x = \"" + className + "\"; return x; };";

		instanceField += "\tconst std::string " + className + "::ThisName = \"" + className + "\";\n";

		if (className != "IObject")
		{
			//add base class properties
			strPropertyInfo += "\t\t// IObject\n";
			
			for (std::map<std::string, NFClassProperty*>::iterator itProperty = pBaseObject->xStructData.xPropertyList.begin();
					itProperty != pBaseObject->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tstatic const std::string& " + propertyName + "(){ static std::string x = \"" + propertyName + "\"; return x; };";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";

				instanceField += "\tconst std::string " + className + "::" + propertyName + " = \"" + propertyName + "\";\n";
			}
		}

		strPropertyInfo += "\t\t// Property\n";
		for (std::map<std::string, NFClassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& propertyName = itProperty->first;
			NFClassProperty* pClassProperty = itProperty->second;

			strPropertyInfo += "\t\tstatic const std::string& " + propertyName + "(){ static std::string x = \"" + propertyName + "\"; return x; };";
			strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";

			instanceField += "\tconst std::string " + className + "::" + propertyName + " = \"" + propertyName + "\";\n";
		}


		//include property, come from 


		for (std::string item : pClassDta->includes)
		{
			auto includeClass = mxClassData[item];

			strPropertyInfo += "\t\t// Include Property, come from " + includeClass->xStructData.className + " \n";

			for (std::map<std::string, NFClassProperty*>::iterator itProperty = includeClass->xStructData.xPropertyList.begin();
				itProperty != includeClass->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tstatic const std::string& " + propertyName + "(){ static std::string x = \"" + propertyName + "\"; return x; };";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";

				instanceField += "\tconst std::string " + className + "::" + propertyName + " = \"" + propertyName + "\";\n";
			}
		}

		fwrite(strPropertyInfo.c_str(), strPropertyInfo.length(), 1, hppWriter);

		//record
		std::string strRecordInfo = "";
		strRecordInfo += "\t\t// Record\n";

		for (std::map<std::string, NFClassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& recordName = itRecord->first;
			NFClassRecord* pClassRecord = itRecord->second;

			std::cout << "save for cpppppp ---> " << className  << "::" << recordName << std::endl;

			strRecordInfo += "\t\tclass " + recordName + "\n\t\t{\n\t\tpublic:\n";
			strRecordInfo += "\t\t\t//Class name\n\t";
			strRecordInfo += "\t\tstatic const std::string& ThisName(){ static std::string x = \"" + recordName + "\"; return x; };\n";

			instanceField += "\tconst std::string " + className + "::" + recordName + "::ThisName = \"" + recordName + "\";\n";

			//col
			for (int i = 0; i < pClassRecord->colList.size(); ++i)
			{
				for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
					itCol != pClassRecord->colList.end(); ++itCol)
				{
					const std::string& colTag = itCol->first;
					NFClassRecord::RecordColDesc* pRecordColDesc = itCol->second;

					if (pRecordColDesc->index == i)
					{
						strRecordInfo += "\t\t\tstatic const int " + colTag + " = " + std::to_string(pRecordColDesc->index) + ";//" + pRecordColDesc->type + "\n";
					}
				}
			}

			strRecordInfo += "\n\t\t};\n";

		}

		//include record

		for (std::string item : pClassDta->includes)
		{
			auto includeClass = mxClassData[item];

			strPropertyInfo += "\t\t// Include Record, come from " + includeClass->xStructData.className + " \n";
			
			for (std::map<std::string, NFClassRecord*>::iterator itRecord = includeClass->xStructData.xRecordList.begin();
				itRecord != includeClass->xStructData.xRecordList.end(); ++itRecord)
			{
				const std::string& recordName = itRecord->first;
				NFClassRecord* pClassRecord = itRecord->second;

				strRecordInfo += "\t\tclass " + recordName + "\n\t\t{\n\t\tpublic:\n";
				strRecordInfo += "\t\t\t//Class name\n\t";
				strRecordInfo += "\t\tstatic const std::string& ThisName(){ static std::string x = \"" + recordName + "\"; return x; };\n";

				instanceField += "\tconst std::string " + className + "::" + recordName + "::ThisName = \"" + recordName + "\";\n";

				//col
				for (int i = 0; i < pClassRecord->colList.size(); ++i)
				{
					for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
						itCol != pClassRecord->colList.end(); ++itCol)
					{
						const std::string& colTag = itCol->first;
						NFClassRecord::RecordColDesc* pRecordColDesc = itCol->second;

						if (pRecordColDesc->index == i)
						{
							strRecordInfo += "\t\t\tstatic const int " + colTag + " = " + std::to_string(pRecordColDesc->index) + ";//" + pRecordColDesc->type + "\n";
						}
					}
				}

				strRecordInfo += "\n\t\t};\n";

			}
		}

		fwrite(strRecordInfo.c_str(), strRecordInfo.length(), 1, hppWriter);

		std::string strHppEnumInfo = "";


		std::string strClassEnd;
		strClassEnd += "\n\t};\n";

		fwrite(strClassEnd.c_str(), strClassEnd.length(), 1, hppWriter);

	}

	//fwrite(instanceField.c_str(), instanceField.length(), 1, hppWriter);

	std::string strFileEnd = "\n}\n#endif";
	fwrite(strFileEnd.c_str(), strFileEnd.length(), 1, hppWriter);
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////

	fclose(hppWriter);

	return false;
}

bool NFFileProcess::SaveForCS()
{
	FILE* csWriter = fopen(strCSFile.c_str(), "w");

	std::string strFileHead = "// -------------------------------------------------------------------------\n";
	strFileHead = strFileHead
		+ "//    @FileName         :    NFProtocolDefine.cs\n"
		+ "//    @Author           :    NFrame Studio\n"
		+ "//    @Module           :    NFProtocolDefine\n"
		+ "// -------------------------------------------------------------------------\n\n"
		+ "using System;\n"
		//+ "using System.Collections.Concurrent;\n"
		+ "using System.Collections.Generic;\n"
		+ "using System.Linq;\n"
		+ "using System.Text;\n"
		+ "using System.Threading;\n"
		//+ "using System.Threading.Tasks;\n\n"
		+ "namespace NFrame\n{\n";

	fwrite(strFileHead.c_str(), strFileHead.length(), 1, csWriter);
	/////////////////////////////////////////////////////


	ClassData* pBaseObject = mxClassData["IObject"];
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		if (it->second->bePartialed)
		{
			continue;
		}

		const std::string& className = it->first;
		ClassData* pClassDta = it->second;
		// cpp
		std::string strPropertyInfo;

		strPropertyInfo += "\tpublic class " + className + "\n\t{\n";
		strPropertyInfo += "\t\t//Class name\n\t";
		strPropertyInfo += "\tpublic static readonly String ThisName = \"" + className + "\";\n";

		if (className != "IObject")
		{
			//add base class properties
			strPropertyInfo += "\t\t// IObject\n";

			for (std::map<std::string, NFClassProperty*>::iterator itProperty = pBaseObject->xStructData.xPropertyList.begin();
				itProperty != pBaseObject->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tpublic static readonly String " + propertyName + " = \"" + propertyName + "\";";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
			}
		}


		strPropertyInfo += "\t\t// Property\n";
		for (std::map<std::string, NFClassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& propertyName = itProperty->first;
			NFClassProperty* pClassProperty = itProperty->second;

			strPropertyInfo += "\t\tpublic static readonly String " + propertyName + " = \"" + propertyName + "\";";
			strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
		}

		//include property, come from 


		for (std::string item : pClassDta->includes)
		{
			auto includeClass = mxClassData[item];

			strPropertyInfo += "\t\t// Include Property, come from " + includeClass->xStructData.className + " \n";

			for (std::map<std::string, NFClassProperty*>::iterator itProperty = includeClass->xStructData.xPropertyList.begin();
				itProperty != includeClass->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tpublic static readonly String " + propertyName + " = \"" + propertyName + "\";";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
			}

		}

		fwrite(strPropertyInfo.c_str(), strPropertyInfo.length(), 1, csWriter);

		//record
		std::string strRecordInfo = "";
		strRecordInfo += "\t\t// Record\n";

		for (std::map<std::string, NFClassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& recordName = itRecord->first;
			NFClassRecord* pClassRecord = itRecord->second;

			std::cout << "save for cs ---> " << className << "::" << recordName << std::endl;

			strRecordInfo += "\t\tpublic class " + recordName + "\n\t\t{\n";
			strRecordInfo += "\t\t\t//Class name\n\t";
			strRecordInfo += "\t\tpublic static readonly String ThisName = \"" + recordName + "\";\n";

			//col
			for (int i = 0; i < pClassRecord->colList.size(); ++i)
			{
				for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
					itCol != pClassRecord->colList.end(); ++itCol)
				{
					const std::string& colTag = itCol->first;
					NFClassRecord::RecordColDesc* pRecordColDesc = itCol->second;

					if (pRecordColDesc->index == i)
					{
						strRecordInfo += "\t\t\tpublic const int " + colTag + " = " + std::to_string(pRecordColDesc->index) + ";//" + pRecordColDesc->type + "\n";
					}
				}
			}
			

			strRecordInfo += "\n\t\t}\n";

		}
		//include record

		for (std::string item : pClassDta->includes)
		{
			auto includeClass = mxClassData[item];

			strPropertyInfo += "\t\t// Include Record, come from " + includeClass->xStructData.className + " \n";

			for (std::map<std::string, NFClassRecord*>::iterator itRecord = includeClass->xStructData.xRecordList.begin();
				itRecord != includeClass->xStructData.xRecordList.end(); ++itRecord)
			{
				const std::string& recordName = itRecord->first;
				NFClassRecord* pClassRecord = itRecord->second;

				std::cout << "save for cs ---> " << className << "::" << recordName << std::endl;

				strRecordInfo += "\t\tpublic class " + recordName + "\n\t\t{\n";
				strRecordInfo += "\t\t\t//Class name\n\t";
				strRecordInfo += "\t\tpublic static readonly String ThisName = \"" + recordName + "\";\n";

				//col
				for (int i = 0; i < pClassRecord->colList.size(); ++i)
				{
					for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
						itCol != pClassRecord->colList.end(); ++itCol)
					{
						const std::string& colTag = itCol->first;
						NFClassRecord::RecordColDesc* pRecordColDesc = itCol->second;

						if (pRecordColDesc->index == i)
						{
							strRecordInfo += "\t\t\tpublic const int " + colTag + " = " + std::to_string(pRecordColDesc->index) + ";//" + pRecordColDesc->type + "\n";
						}
					}
				}


				strRecordInfo += "\n\t\t}\n";

			}
		}

		fwrite(strRecordInfo.c_str(), strRecordInfo.length(), 1, csWriter);

		std::string strHppEnumInfo = "";


		std::string strClassEnd;
		strClassEnd += "\n\t}\n";

		fwrite(strClassEnd.c_str(), strClassEnd.length(), 1, csWriter);

	}

	std::string strFileEnd = "\n}";
	fwrite(strFileEnd.c_str(), strFileEnd.length(), 1, csWriter);

	fclose(csWriter);

	return false;
}
bool NFFileProcess::SaveForTS()
{
	FILE* csWriter = fopen(strTSFile.c_str(), "w");

	std::string strFileHead = "// -------------------------------------------------------------------------\n";
	strFileHead = strFileHead
		+ "//    @FileName         :    NFProtocolDefine.ts\n"
		+ "//    @Author           :    NFrame Studio\n"
		+ "//    @Module           :    NFProtocolDefine\n"
		+ "// -------------------------------------------------------------------------\n\n";
	fwrite(strFileHead.c_str(), strFileHead.length(), 1, csWriter);
	/////////////////////////////////////////////////////


	ClassData* pBaseObject = mxClassData["IObject"];
	std::string allClassNames = "export const NFConfig={\n\t\t";
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		if (it->second->bePartialed)
		{
			continue;
		}

		const std::string& className = it->first;
		ClassData* pClassDta = it->second;
		// cpp
		std::string strPropertyInfo;
		if(it != mxClassData.begin()){
			allClassNames+=',';
		}
		allClassNames+=className;

		strPropertyInfo += "\tclass " + className + "\n\t{\n";
		strPropertyInfo += "\t\t//Class name\n\t";
		strPropertyInfo += "\tpublic static  ThisName = \"" + className + "\";\n";
		if (className != "IObject")
		{
			//add base class properties
			strPropertyInfo += "\t\t// IObject\n";

			for (std::map<std::string, NFClassProperty*>::iterator itProperty = pBaseObject->xStructData.xPropertyList.begin();
				itProperty != pBaseObject->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tpublic static " + propertyName + " = \"" + propertyName + "\";";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
			}
		}


		strPropertyInfo += "\t\t// Property\n";
		for (std::map<std::string, NFClassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& propertyName = itProperty->first;
			NFClassProperty* pClassProperty = itProperty->second;

			strPropertyInfo += "\t\tpublic static " + propertyName + " = \"" + propertyName + "\";";
			strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
		}


		//include property, come from 


		for (std::string item : pClassDta->includes)
		{
			auto includeClass = mxClassData[item];

			strPropertyInfo += "\t\t// Include Property, come from " + includeClass->xStructData.className + " \n";

			for (std::map<std::string, NFClassProperty*>::iterator itProperty = includeClass->xStructData.xPropertyList.begin();
				itProperty != includeClass->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tpublic static " + propertyName + " = \"" + propertyName + "\";";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
			}
		}

		fwrite(strPropertyInfo.c_str(), strPropertyInfo.length(), 1, csWriter);

		//record
		std::string strRecordInfo = "";
		strRecordInfo += "\t\t// Record\n";

		for (std::map<std::string, NFClassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& recordName = itRecord->first;
			NFClassRecord* pClassRecord = itRecord->second;

			std::cout << "save for cs ---> " << className << "::" << recordName << std::endl;

			strRecordInfo += "\t\tpublic static " + recordName + " = \n\t\t{\n";
			strRecordInfo += "\t\t\t//Class name\n\t";
			strRecordInfo += "\t\t\"ThisName\":\"" + recordName + "\",\n";

			//col
			for (int i = 0; i < pClassRecord->colList.size(); ++i)
			{
				for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
					itCol != pClassRecord->colList.end(); ++itCol)
				{

					const std::string& colTag = itCol->first;
					NFClassRecord::RecordColDesc* pRecordColDesc = itCol->second;

					if (pRecordColDesc->index == i)
					{
						if(i!= 0){
							strRecordInfo+=",\n";
						}
						strRecordInfo += "\t\t\t\"" + colTag + "\":" + std::to_string(pRecordColDesc->index);
					}
				}
			}
			

			strRecordInfo += "\n\t\t}\n";

		}

		for (std::string item : pClassDta->includes)
		{
			auto includeClass = mxClassData[item];

			strRecordInfo += "\t\t// Include Record, come from " + includeClass->xStructData.className + " \n";

			for (std::map<std::string, NFClassRecord*>::iterator itRecord = includeClass->xStructData.xRecordList.begin();
				itRecord != includeClass->xStructData.xRecordList.end(); ++itRecord)
			{
				const std::string& recordName = itRecord->first;
				NFClassRecord* pClassRecord = itRecord->second;

				std::cout << "save for cs ---> " << className << "::" << recordName << std::endl;

				strRecordInfo += "\t\tpublic static " + recordName + " = \n\t\t{\n";
				strRecordInfo += "\t\t\t//Class name\n\t";
				strRecordInfo += "\t\t\"ThisName\":\"" + recordName + "\",\n";

				//col
				for (int i = 0; i < pClassRecord->colList.size(); ++i)
				{
					for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
						itCol != pClassRecord->colList.end(); ++itCol)
					{

						const std::string& colTag = itCol->first;
						NFClassRecord::RecordColDesc* pRecordColDesc = itCol->second;

						if (pRecordColDesc->index == i)
						{
							if (i != 0) {
								strRecordInfo += ",\n";
							}
							strRecordInfo += "\t\t\t\"" + colTag + "\":" + std::to_string(pRecordColDesc->index);
						}
					}
				}


				strRecordInfo += "\n\t\t}\n";

			}
		}

		fwrite(strRecordInfo.c_str(), strRecordInfo.length(), 1, csWriter);

		std::string strHppEnumInfo = "";


		std::string strClassEnd;
		strClassEnd += "\n\t}\n";

		fwrite(strClassEnd.c_str(), strClassEnd.length(), 1, csWriter);

	}
	allClassNames+="}\n";
	fwrite(allClassNames.c_str(), allClassNames.length(), 1, csWriter);
	fclose(csWriter);

	return false;
}

bool NFFileProcess::SaveForJAVA()
{
	FILE* javaWriter = fopen(strJavaFile.c_str(), "w");

	std::string strFileHead
		= "// -------------------------------------------------------------------------\n";
	strFileHead = strFileHead
		+ "//    @FileName         :    NFProtocolDefine.java\n"
		+ "//    @Author           :    NFrame Studio\n"
		+ "//    @Module           :    NFProtocolDefine\n"
		+ "// -------------------------------------------------------------------------\n\n"
		+ "package nframe;\n";

	fwrite(strFileHead.c_str(), strFileHead.length(), 1, javaWriter);
	/////////////////////////////////////////////////////

	ClassData* pBaseObject = mxClassData["IObject"];
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		if (it->second->bePartialed)
		{
			continue;
		}

		const std::string& className = it->first;
		ClassData* pClassDta = it->second;
		// cpp
		std::string strPropertyInfo;

		strPropertyInfo += "\tpublic class " + className + "\n\t{\n";
		strPropertyInfo += "\t\t//Class name\n\t";
		strPropertyInfo += "\tpublic static final String ThisName = \"" + className + "\";\n";

		if (className != "IObject")
		{
			//add base class properties
			strPropertyInfo += "\t\t// IObject\n";

			for (std::map<std::string, NFClassProperty*>::iterator itProperty = pBaseObject->xStructData.xPropertyList.begin();
				itProperty != pBaseObject->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tpublic static final String " + propertyName + " = \"" + propertyName + "\";";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
			}
		}

		strPropertyInfo += "\t\t// Property\n";
		for (std::map<std::string, NFClassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& propertyName = itProperty->first;
			NFClassProperty* pClassProperty = itProperty->second;

			strPropertyInfo += "\t\tpublic static final String " + propertyName + " = \"" + propertyName + "\";";
			strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
		}

		fwrite(strPropertyInfo.c_str(), strPropertyInfo.length(), 1, javaWriter);

		//record
		std::string strRecordInfo = "";
		strRecordInfo += "\t\t// Record\n";

		for (std::map<std::string, NFClassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& recordName = itRecord->first;
			NFClassRecord* pClassRecord = itRecord->second;

			std::cout << "save for java ---> " << className << "::" << recordName << std::endl;

			strRecordInfo += "\t\tpublic class " + recordName + "\n\t\t{\n";
			strRecordInfo += "\t\t\t//Class name\n\t";
			strRecordInfo += "\t\tpublic static final String ThisName = \"" + recordName + "\";\n";

			//col
			for (int i = 0; i < pClassRecord->colList.size(); ++i)
			{
				for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
					itCol != pClassRecord->colList.end(); ++itCol)
				{
					const std::string& colTag = itCol->first;
					NFClassRecord::RecordColDesc* pRecordColDesc = itCol->second;

					if (pRecordColDesc->index == i)
					{
						strRecordInfo += "\t\t\tpublic static final int " + colTag + " = " + std::to_string(pRecordColDesc->index) + ";//" + pRecordColDesc->type + "\n";
					}
				}
			}

			strRecordInfo += "\n\t\t}\n";

		}
		fwrite(strRecordInfo.c_str(), strRecordInfo.length(), 1, javaWriter);

		std::string strHppEnumInfo = "";


		std::string strClassEnd;
		strClassEnd += "\n\t}\n";

		fwrite(strClassEnd.c_str(), strClassEnd.length(), 1, javaWriter);

	}

	fclose(javaWriter);

	return false;
}

bool NFFileProcess::SaveForPB()
{

	return false;
}

bool NFFileProcess::SaveForSQL()
{
	//1 class.sql
	std::string fileName = strXMLStructPath + "NFrame.sql";

	FILE* iniWriter = fopen(fileName.c_str(), "w");

	std::string strElementData;
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		const std::string& className = it->first;
		ClassData* pClassDta = it->second;
		if (className == "IObject")
		{
			continue;
		}
		if (it->second->beIncluded)
		{
			//continue;
		}

		strElementData += "CREATE TABLE IF NOT EXISTS " + pClassDta->xStructData.className + "  (";
		strElementData += " `ID` varchar(128) NOT NULL,";
		strElementData += " PRIMARY KEY (`ID`)";
		strElementData += " ) ENGINE=InnoDB DEFAULT CHARSET=utf8; \n";
	}


	//2 fields
	//ALTER TABLE `Buff` ADD `EffectType` bigint(11) DEFAULT '0' COMMENT '影响属性类型(效果类型)  生命，法力(可组合,叠加)';
	// 2.1) property
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		const std::string& className = it->first;
		ClassData* pClassDta = it->second;

		for (std::map<std::string, NFClassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& propertyName = itProperty->first;
			NFClassProperty* xPropertyData = itProperty->second;

			std::string strType;
			std::string strSave;
			std::string strCache;
			std::string strDesc;
			for (std::map<std::string, std::string>::iterator itDesc = xPropertyData->descList.begin();
				itDesc != xPropertyData->descList.end(); ++itDesc)
			{
				const std::string& strKey = itDesc->first;
				const std::string& value = itDesc->second;
				if (strKey=="Type")
				{
					strType = value;
				}
				else if (strKey=="Save")
				{
					strSave = value;
				} 
				else if (strKey=="Cache")
				{
					strCache = value;
				}
				else if (strKey=="Desc")
				{
					strDesc = value;
				}
			}

			if (strSave=="1" || strCache=="1")
			{
				std::string strAlter = "\nALTER TABLE `" + className + "` ADD `" + propertyName + "`";
				//ALTER TABLE `Buff` ADD `EffectType` bigint(11) DEFAULT '0' COMMENT '影响属性类型(效果类型)  生命，法力(可组合,叠加)';
				if (strType=="int")
				{
					strAlter += " bigint(11) DEFAULT '0'";
				}
				else if (strType=="string")
				{
					strAlter += " text COLLATE utf8mb4_unicode_ci  DEFAULT ''";
				}
				else if (strType=="float")
				{
					strAlter += " double DEFAULT '0.0'";
				}
				else if (strType=="object")
				{
					strAlter += " varchar(64) COLLATE utf8mb4_unicode_ci DEFAULT ''";
				}
				else if (strType=="vector2")
				{
					strAlter += " varchar(64) COLLATE utf8mb4_unicode_ci DEFAULT ''";
				}
				else if (strType=="vector3")
				{
					strAlter += " varchar(64) COLLATE utf8mb4_unicode_ci DEFAULT ''";
				}

				strAlter += " COMMENT '" + strDesc + "';";

				strElementData += strAlter;
			}
		}

		// 2.2) record
		for (std::map<std::string, NFClassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& recordName = itRecord->first;
			NFClassRecord* xRecordData = itRecord->second;

			std::string strType;
			std::string strSave;
			std::string strCache;
			std::string strDesc;
			for (std::map<std::string, std::string>::iterator itDesc = xRecordData->descList.begin();
				itDesc != xRecordData->descList.end(); ++itDesc)
			{
				const std::string& strKey = itDesc->first;
				const std::string& value = itDesc->second;
				if (strKey=="Type")
				{
					strType = value;
				}
				else if (strKey=="Save")
				{
					strSave = value;
				} 
				else if (strKey=="Cache")
				{
					strCache = value;
				}
				else if (strKey=="Desc")
				{
					strDesc = value;
				}
			}

			if (strSave=="1" || strCache=="1")
			{
				std::string strAlter = "\nALTER TABLE `" + className + "` ADD `" + recordName + "`";
				strAlter += " text COLLATE utf8mb4_unicode_ci  DEFAULT ''";
				strAlter += " COMMENT '" + strDesc + "';";

				strElementData += strAlter;
			}
		}
	}
	
	fwrite(strElementData.c_str(), strElementData.length(), 1, iniWriter);

	return true;
}

bool NFFileProcess::SaveForStruct()
{
	ClassData* pBaseObject = mxClassData["IObject"];

	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		if (it->second->bePartialed)
		{
			continue;
		}

		const std::string& className = it->first;
		ClassData* pClassDta = it->second;

		std::cout << "save for struct ---> " << className << std::endl;
		std::string path = pClassDta->filePath;
		StringReplace(path, strExcelIniPath, "");
		std::string fileName = strXMLStructPath + path + ".xml";

		FILE* structWriter = fopen(fileName.c_str(), "w+");
		if (structWriter == nullptr)
		{
			std::string folder = pClassDta->fileFolder;
			StringReplace(folder, strExcelIniPath, "");
			std::string fileFolder = strXMLStructPath + folder;

#if NF_PLATFORM == NF_PLATFORM_WIN
			mkdir(fileFolder.c_str());
#else
			mkdir(fileFolder.c_str(), 0777);
#endif

			structWriter = fopen(fileName.c_str(), "w+");
		}

		if (structWriter)
		{
			std::string strFileHead = "<?xml version='1.0' encoding='utf-8' ?>\n<XML>\n";
			fwrite(strFileHead.c_str(), strFileHead.length(), 1, structWriter);
			/////////////////////////
			std::string strFilePropertyBegin = "\t<Propertys>\n";
			fwrite(strFilePropertyBegin.c_str(), strFilePropertyBegin.length(), 1, structWriter);

			for (std::map<std::string, NFClassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
				itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& propertyName = itProperty->first;
				NFClassProperty* xPropertyData = itProperty->second;

				std::string strElementData = "\t\t<Property Id=\"" + propertyName + "\" ";
				for (std::map<std::string, std::string>::iterator itDesc = xPropertyData->descList.begin();
					itDesc != xPropertyData->descList.end(); ++itDesc)
				{
					const std::string& strKey = itDesc->first;
					const std::string& value = itDesc->second;
					strElementData += strKey + "=\"" + value + "\" ";
				}
				strElementData += "/>\n";
				fwrite(strElementData.c_str(), strElementData.length(), 1, structWriter);
			}

			std::string strFilePropertyEnd = "\t</Propertys>\n";
			fwrite(strFilePropertyEnd.c_str(), strFilePropertyEnd.length(), 1, structWriter);
			//////////////////////////////

			std::string strFileRecordBegin = "\t<Records>\n";
			fwrite(strFileRecordBegin.c_str(), strFileRecordBegin.length(), 1, structWriter);

			for (std::map<std::string, NFClassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
				itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
			{
				const std::string& recordName = itRecord->first;
				NFClassRecord* xRecordData = itRecord->second;

				//for desc
				std::string strElementData = "\t\t<Record Id=\"" + recordName + "\" ";
				for (std::map<std::string, std::string>::iterator itDesc = xRecordData->descList.begin();
					itDesc != xRecordData->descList.end(); ++itDesc)
				{
					const std::string& strKey = itDesc->first;
					const std::string& value = itDesc->second;
					strElementData += strKey + "=\"" + value + "\"\t ";
				}
				strElementData += ">\n";

				//for col list
				for (int i = 0; i < xRecordData->colList.size(); ++i)
				{
					for (std::map<std::string, NFClassRecord::RecordColDesc*>::iterator itDesc = xRecordData->colList.begin();
						itDesc != xRecordData->colList.end(); ++itDesc)
					{
						const std::string& strKey = itDesc->first;
						const NFClassRecord::RecordColDesc* pRecordColDesc = itDesc->second;

						if (pRecordColDesc->index == i)
						{
							strElementData += "\t\t\t<Col Type =\"" + pRecordColDesc->type + "\"\tTag=\"" + strKey + "\"/>";
							if (!pRecordColDesc->desc.empty())
							{
								strElementData += "<!--- " + pRecordColDesc->desc + "-->\n";
							}
							else
							{
								strElementData += "\n";
							}
						}
					}

				}

				strElementData += "\t\t</Record>\n";
				fwrite(strElementData.c_str(), strElementData.length(), 1, structWriter);
			}

			std::string strFileRecordEnd = "\t</Records>\n";
			fwrite(strFileRecordEnd.c_str(), strFileRecordEnd.length(), 1, structWriter);

			/////////////////////////////////

			std::string strFileIncludeBegin = "\t<Includes>\n";
			fwrite(strFileIncludeBegin.c_str(), strFileIncludeBegin.length(), 1, structWriter);


			std::string strFileIncludeBody;
			for (auto item : pClassDta->includes)
			{
				auto includeClass = mxClassData[item];

				std::string path = includeClass->filePath;
				StringReplace(path, strExcelIniPath, "");
				std::string fileName = "NFDataCfg/Struct/" + path + ".xml";

				strFileIncludeBody += "\t\t<Include Id=\"" + fileName + "\" />";
				strFileIncludeBody += "\n";
			}

			fwrite(strFileIncludeBody.c_str(), strFileIncludeBody.length(), 1, structWriter);

			std::string strFileIncludeEnd = "\t</Includes>\n";
			fwrite(strFileIncludeEnd.c_str(), strFileIncludeEnd.length(), 1, structWriter);


			/////////////////////////////////
			std::string strFileEnd = "</XML>";
			fwrite(strFileEnd.c_str(), strFileEnd.length(), 1, structWriter);
		}
		else
		{
			std::cout << "save for struct error!!!!!---> " << fileName << std::endl;
		}
	}

	return false;
}

bool NFFileProcess::SaveForIni()
{

	ClassData* pBaseObject = mxClassData["IObject"];
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		if (it->second->bePartialed)
		{
			continue;
		}

		const std::string& className = it->first;
		ClassData* pClassDta = it->second;

		std::cout << "save for ini ---> " << className << std::endl;

		std::string path = pClassDta->filePath;
		StringReplace(path, strExcelIniPath, "");
		std::string fileName = strXMLIniPath + path + ".xml";

		FILE* iniWriter = fopen(fileName.c_str(), "w+");
		if (iniWriter == nullptr)
		{
			std::string folder = pClassDta->fileFolder;
			StringReplace(folder, strExcelIniPath, "");
			std::string fileFolder = strXMLIniPath + folder;

#if NF_PLATFORM == NF_PLATFORM_WIN
			mkdir(fileFolder.c_str());
#else
			mkdir(fileFolder.c_str(), 0777);
#endif

			iniWriter = fopen(fileName.c_str(), "w+");
		}
		if (iniWriter)
		{
			std::string strFileHead = "<?xml version='1.0' encoding='utf-8' ?>\n<XML>\n";
			fwrite(strFileHead.c_str(), strFileHead.length(), 1, iniWriter);

			for (std::map<std::string, NFClassElement::ElementData*>::iterator itElement = pClassDta->xIniData.xElementList.begin();
				itElement != pClassDta->xIniData.xElementList.end(); ++itElement)
			{

				const std::string& strElementName = itElement->first;
				NFClassElement::ElementData* pIniData = itElement->second;

				std::string strElementData = "\t<Object Id=\"" + strElementName + "\" ";
				for (std::map<std::string, std::string>::iterator itProperty = pIniData->xPropertyList.begin();
					itProperty != pIniData->xPropertyList.end(); ++itProperty)
				{
					const std::string& strKey = itProperty->first;
					const std::string& value = itProperty->second;
					strElementData += strKey + "=\"" + value + "\" ";
				}
				strElementData += "/>\n";





				fwrite(strElementData.c_str(), strElementData.length(), 1, iniWriter);
			}

			std::string strFileEnd = "</XML>";
			fwrite(strFileEnd.c_str(), strFileEnd.length(), 1, iniWriter);
		}
		else
		{
			std::cout << "save for ini error!!!!!---> " << fileName << std::endl;
		}
	}

	return false;
}

bool NFFileProcess::SaveForLogicClass()
{
	std::string fileName = strXMLStructPath + "LogicClass.xml";

	FILE* iniWriter = fopen(fileName.c_str(), "w");

	std::string strFileHead = "<?xml version='1.0' encoding='utf-8' ?>\n<XML>\n";
	fwrite(strFileHead.c_str(), strFileHead.length(), 1, iniWriter);

	ClassData* pBaseObject = mxClassData["IObject"];

	std::string strElementData;
	strElementData += "\t<Class Id=\"" + pBaseObject->xStructData.className + "\"\t";

	std::string path = pBaseObject->filePath;
	StringReplace(path, strExcelIniPath, "");

	strElementData += "Path=\"NFDataCfg/Struct/" + path + ".xml\"\t";
	strElementData += "InstancePath=\"NFDataCfg/Ini/" + path + ".xml\"\t>\n";


	//strElementData += "Path=\"NFDataCfg/Struct/" + pBaseObject->xStructData.className + ".xml\"\t";
	//strElementData += "InstancePath=\"NFDataCfg/Ini/" + pBaseObject->xStructData.className + ".xml\"\t>\n";

	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		const std::string& className = it->first;
		ClassData* pClassDta = it->second;
		if (className == "IObject")
		{
			continue;
		}

		if (it->second->bePartialed)
		{
			continue;
		}

		strElementData += "\t\t<Class Id=\"" + pClassDta->xStructData.className + "\"\t";

		std::string path = pClassDta->filePath;
		StringReplace(path, strExcelIniPath, "");

		strElementData += "Path=\"NFDataCfg/Struct/" + path + ".xml\"\t";
		strElementData += "InstancePath=\"NFDataCfg/Ini/" + path + ".xml\"\t/>\n";

		//strElementData += "Path=\"NFDataCfg/Struct/" + pClassDta->xStructData.className + ".xml\"\t";
		//strElementData += "InstancePath=\"NFDataCfg/Ini/" + pClassDta->xStructData.className + ".xml\"\t/>\n";

	}

	strElementData += "\t</Class>\n";
	fwrite(strElementData.c_str(), strElementData.length(), 1, iniWriter);

	std::string strFileEnd = "</XML>";
	fwrite(strFileEnd.c_str(), strFileEnd.length(), 1, iniWriter);

	return false;
}


void NFFileProcess::SetUTF8(const bool b)
{
	bConvertIntoUTF8 = b;
}

std::string NFFileProcess::GetFileNameByPath(const std::string& filePath)
{
	std::string fileName = filePath.substr(filePath.find_last_of('/') + 1, filePath.find_last_of('.') - filePath.find_last_of('/') - 1);
	return fileName;
}

std::string NFFileProcess::GetFileNameExtByPath(const std::string& filePath)
{
	std::string ext = filePath.substr(filePath.find_last_of('.'), filePath.length() - filePath.find_last_of('.'));
	return ext;
}

std::vector<std::string> NFFileProcess::GetFileListInFolder(std::string folderPath, int depth)
{
	depth--;

	std::vector<std::string> result;
#if NF_PLATFORM == NF_PLATFORM_WIN
	_finddata_t FileInfo;
	std::string strfind = folderPath + "\\*";
	long long Handle = _findfirst(strfind.c_str(), &FileInfo);
	if (Handle == -1L)
	{
		//std::cerr << "can not match the folder path:" << folderPath << std::endl;
		return result;
	}
	do {

		if (FileInfo.attrib & _A_SUBDIR)
		{

			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				std::string newPath = folderPath + "\\" + FileInfo.name;
				if (depth >= 0)
				{
					std::vector<std::string> childResult = GetFileListInFolder(newPath, depth);
					result.insert(result.end(), childResult.begin(), childResult.end());
				}
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
	if (pDir)
	{
		memset(childpath, 0, sizeof(childpath));
		while ((ent = readdir(pDir)) != NULL)
		{
			if (ent->d_type & DT_DIR)
			{
				if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
				{
					sprintf(absolutepath, "%s/%s", folderPath.c_str(), ent->d_name);
					if (depth >= 0)
					{
						std::vector<std::string> childResult = GetFileListInFolder(absolutepath, depth);
						result.insert(result.end(), childResult.begin(), childResult.end());
					}
				}
			}
			else
			{
				sprintf(absolutepath, "%s/%s", folderPath.c_str(), ent->d_name);
				result.push_back(absolutepath);
			}
		}

		sort(result.begin(), result.end());
	}
	
#endif
	return result;
}

std::vector<std::string> NFFileProcess::GetFolderListInFolder(std::string folderPath)
{
	std::vector<std::string> result;
#if NF_PLATFORM == NF_PLATFORM_WIN
	_finddata_t FileInfo;
	std::string strfind = folderPath + "\\*";
	long long Handle = _findfirst(strfind.c_str(), &FileInfo);
	if (Handle == -1L)
	{
		std::cerr << "can not match the folder path" << std::endl;
		return result;
	}
	do {

		if (FileInfo.attrib & _A_SUBDIR)
		{
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				std::string newPath = folderPath + "\\" + FileInfo.name;
				result.push_back(FileInfo.name);
			}
		}
		
	} while (_findnext(Handle, &FileInfo) == 0);


	_findclose(Handle);
#else
	DIR* pDir;
	struct dirent* ent;
	char childpath[512];
	char absolutepath[512];
	pDir = opendir(folderPath.c_str());
	memset(childpath, 0, sizeof(childpath));
	while ((ent = readdir(pDir)) != NULL)
	{
		if (ent->d_type & DT_DIR)
		{
			if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
			{
				sprintf(absolutepath, "%s/%s", folderPath.c_str(), ent->d_name);

				result.push_back(ent->d_name);
			}
		}
	}

	sort(result.begin(), result.end());
#endif
	return result;
}

void NFFileProcess::StringReplace(std::string & strBig, const std::string & strsrc, const std::string & strdst)
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