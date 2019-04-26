#include "NFFileProcess.h"
#include "Utf8ToGbk.h"
#include <iostream>
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


NFFileProcess::NFFileProcess()
{

}

NFFileProcess::~NFFileProcess()
{

}

bool NFFileProcess::LoadDataFromExcel()
{
	LoadDataFromExcel("../Excel/IObject.xlsx", "IObject");


	auto fileList = GetFileListInFolder(strExcelIniPath, 0);

	for (auto fileName : fileList)
	{
		StringReplace(fileName, "\\", "/");
		StringReplace(fileName, "//", "/");

		if ((int)(fileName.find("$")) != -1)
		{
			continue;
		}


		auto strExt = fileName.substr(fileName.find_last_of('.') + 1, fileName.length() - fileName.find_last_of('.') - 1);
		if (strExt != "xlsx")
		{
			continue;
		}


		auto strFileName = fileName.substr(fileName.find_last_of('/') + 1, fileName.find_last_of('.') - fileName.find_last_of('/') - 1);
		if (strFileName == "IObject")
		{
			continue;
		}


		if (!LoadDataFromExcel(fileName.c_str(), strFileName.c_str()))
		{
			std::cout << "Create " + fileName + " failed!" << std::endl;
			return false;
		}
	}

	return true;
}

bool NFFileProcess::LoadDataFromExcel(const std::string & strFile, const std::string & strFileName)
{
	if (mxClassData.find(strFileName) != mxClassData.end())
	{
		std::cout << strFile << " exist!!!" << std::endl;
		return false;
	}

	std::cout << strFile << std::endl;

	ClassData* pClassData = new ClassData();
	mxClassData[strFileName] = pClassData;
	pClassData->xStructData.strClassName = strFileName;

	///////////////////////////////////////
	//load

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
	else
	{
		std::cout << pClassData->xStructData.strClassName << " " << strSheetName << std::endl;
		assert(0);
	}

	return false;
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
			NFlassElement::ElementData* pIniObject = new NFlassElement::ElementData();
			pClassData->xIniData.xElementList[pIDCell->value] = pIniObject;

			for (std::map<std::string, int>::iterator itProperty = PropertyIndex.begin(); itProperty != PropertyIndex.end(); ++itProperty)
			{
				std::string strPropertyName = itProperty->first;
				int nCol = itProperty->second;

				MiniExcelReader::Cell* cell = sheet.getCell(r, nCol);
				if (cell)
				{
					pIniObject->xPropertyList[strPropertyName] = cell->value;
				}
				else
				{
					pIniObject->xPropertyList[strPropertyName] = "";
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
		std::string strPropertyName = itProperty->first;
		int nCol = itProperty->second;

		////////////
		NFlassProperty* pClassProperty = new NFlassProperty();
		pClassData->xStructData.xPropertyList[strPropertyName] = pClassProperty;

		////////////
		for (std::map<std::string, int>::iterator itDesc = descIndex.begin(); itDesc != descIndex.end(); ++itDesc)
		{
			std::string descName = itDesc->first;
			int nRow = itDesc->second;

			
			MiniExcelReader::Cell* pCell = sheet.getCell(nRow, nCol);
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
			std::string strRecordName = pNameCell->value;
			
			////////////

			NFlassRecord* pClassRecord = new NFlassRecord();
			pClassData->xStructData.xRecordList[strRecordName] = pClassRecord;
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

				NFlassRecord::RecordColDesc* pRecordColDesc = new NFlassRecord::RecordColDesc();
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

bool NFFileProcess::Save()
{
	SaveForCPP();
	SaveForCS();
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
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;
		// cpp
		std::string strPropertyInfo;
		
		strPropertyInfo += "\tclass " + strClassName + "\n\t{\n\tpublic:\n";
		strPropertyInfo += "\t\t//Class name\n\t";
		strPropertyInfo += "\tstatic const std::string& ThisName(){ static std::string x = \"" + strClassName + "\"; return x; };";

		instanceField += "\tconst std::string " + strClassName + "::ThisName = \"" + strClassName + "\";\n";

		if (strClassName != "IObject")
		{
			//add base class properties
			strPropertyInfo += "\t\t// IObject\n";
			
			for (std::map<std::string, NFlassProperty*>::iterator itProperty = pBaseObject->xStructData.xPropertyList.begin();
					itProperty != pBaseObject->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& strPropertyName = itProperty->first;
				NFlassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tstatic const std::string& " + strPropertyName + "(){ static std::string x = \"" + strPropertyName + "\"; return x; };";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";

				instanceField += "\tconst std::string " + strClassName + "::" + strPropertyName + " = \"" + strPropertyName + "\";\n";
			}
		}

		strPropertyInfo += "\t\t// Property\n";
		for (std::map<std::string, NFlassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& strPropertyName = itProperty->first;
			NFlassProperty* pClassProperty = itProperty->second;

			strPropertyInfo += "\t\tstatic const std::string& " + strPropertyName + "(){ static std::string x = \"" + strPropertyName + "\"; return x; };";
			strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";

			instanceField += "\tconst std::string " + strClassName + "::" + strPropertyName + " = \"" + strPropertyName + "\";\n";
		}

		fwrite(strPropertyInfo.c_str(), strPropertyInfo.length(), 1, hppWriter);

		//record
		std::string strRecordInfo = "";
		strRecordInfo += "\t\t// Record\n";

		for (std::map<std::string, NFlassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& strRecordName = itRecord->first;
			NFlassRecord* pClassRecord = itRecord->second;

			std::cout << "save for cpp ---> " << strClassName  << "::" << strRecordName << std::endl;

			strRecordInfo += "\t\tclass " + strRecordName + "\n\t\t{\n\t\tpublic:\n";
			strRecordInfo += "\t\t\t//Class name\n\t";
			strRecordInfo += "\t\tstatic const std::string& ThisName(){ static std::string x = \"" + strRecordName + "\"; return x; };\n";

			instanceField += "\tconst std::string " + strClassName + "::" + strRecordName + "::ThisName = \"" + strRecordName + "\";\n";

			//col
			for (int i = 0; i < pClassRecord->colList.size(); ++i)
			{
				for (std::map<std::string, NFlassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
					itCol != pClassRecord->colList.end(); ++itCol)
				{
					const std::string& colTag = itCol->first;
					NFlassRecord::RecordColDesc* pRecordColDesc = itCol->second;

					if (pRecordColDesc->index == i)
					{
						strRecordInfo += "\t\t\tstatic const int " + colTag + " = " + std::to_string(pRecordColDesc->index) + ";//" + pRecordColDesc->type + "\n";
					}
				}
			}

			strRecordInfo += "\n\t\t};\n";

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
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;
		// cpp
		std::string strPropertyInfo;

		strPropertyInfo += "\tpublic class " + strClassName + "\n\t{\n";
		strPropertyInfo += "\t\t//Class name\n\t";
		strPropertyInfo += "\tpublic static readonly String ThisName = \"" + strClassName + "\";\n";

		if (strClassName != "IObject")
		{
			//add base class properties
			strPropertyInfo += "\t\t// IObject\n";

			for (std::map<std::string, NFlassProperty*>::iterator itProperty = pBaseObject->xStructData.xPropertyList.begin();
				itProperty != pBaseObject->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& strPropertyName = itProperty->first;
				NFlassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tpublic static readonly String " + strPropertyName + " = \"" + strPropertyName + "\";";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
			}
		}


		strPropertyInfo += "\t\t// Property\n";
		for (std::map<std::string, NFlassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& strPropertyName = itProperty->first;
			NFlassProperty* pClassProperty = itProperty->second;

			strPropertyInfo += "\t\tpublic static readonly String " + strPropertyName + " = \"" + strPropertyName + "\";";
			strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
		}

		fwrite(strPropertyInfo.c_str(), strPropertyInfo.length(), 1, csWriter);

		//record
		std::string strRecordInfo = "";
		strRecordInfo += "\t\t// Record\n";

		for (std::map<std::string, NFlassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& strRecordName = itRecord->first;
			NFlassRecord* pClassRecord = itRecord->second;

			std::cout << "save for cs ---> " << strClassName << "::" << strRecordName << std::endl;

			strRecordInfo += "\t\tpublic class " + strRecordName + "\n\t\t{\n";
			strRecordInfo += "\t\t\t//Class name\n\t";
			strRecordInfo += "\t\tpublic static readonly String ThisName = \"" + strRecordName + "\";\n";

			//col
			for (int i = 0; i < pClassRecord->colList.size(); ++i)
			{
				for (std::map<std::string, NFlassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
					itCol != pClassRecord->colList.end(); ++itCol)
				{
					const std::string& colTag = itCol->first;
					NFlassRecord::RecordColDesc* pRecordColDesc = itCol->second;

					if (pRecordColDesc->index == i)
					{
						strRecordInfo += "\t\t\tpublic const int " + colTag + " = " + std::to_string(pRecordColDesc->index) + ";//" + pRecordColDesc->type + "\n";
					}
				}
			}
			

			strRecordInfo += "\n\t\t}\n";

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
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;
		// cpp
		std::string strPropertyInfo;

		strPropertyInfo += "\tpublic class " + strClassName + "\n\t{\n";
		strPropertyInfo += "\t\t//Class name\n\t";
		strPropertyInfo += "\tpublic static final String ThisName = \"" + strClassName + "\";\n";

		if (strClassName != "IObject")
		{
			//add base class properties
			strPropertyInfo += "\t\t// IObject\n";

			for (std::map<std::string, NFlassProperty*>::iterator itProperty = pBaseObject->xStructData.xPropertyList.begin();
				itProperty != pBaseObject->xStructData.xPropertyList.end(); ++itProperty)
			{
				const std::string& strPropertyName = itProperty->first;
				NFlassProperty* pClassProperty = itProperty->second;

				strPropertyInfo += "\t\tpublic static final String " + strPropertyName + " = \"" + strPropertyName + "\";";
				strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
			}
		}

		strPropertyInfo += "\t\t// Property\n";
		for (std::map<std::string, NFlassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& strPropertyName = itProperty->first;
			NFlassProperty* pClassProperty = itProperty->second;

			strPropertyInfo += "\t\tpublic static final String " + strPropertyName + " = \"" + strPropertyName + "\";";
			strPropertyInfo += "// " + pClassProperty->descList["Type"] + "\n";
		}

		fwrite(strPropertyInfo.c_str(), strPropertyInfo.length(), 1, javaWriter);

		//record
		std::string strRecordInfo = "";
		strRecordInfo += "\t\t// Record\n";

		for (std::map<std::string, NFlassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& strRecordName = itRecord->first;
			NFlassRecord* pClassRecord = itRecord->second;

			std::cout << "save for java ---> " << strClassName << "::" << strRecordName << std::endl;

			strRecordInfo += "\t\tpublic class " + strRecordName + "\n\t\t{\n";
			strRecordInfo += "\t\t\t//Class name\n\t";
			strRecordInfo += "\t\tpublic static final String ThisName = \"" + strRecordName + "\";\n";

			//col
			for (int i = 0; i < pClassRecord->colList.size(); ++i)
			{
				for (std::map<std::string, NFlassRecord::RecordColDesc*>::iterator itCol = pClassRecord->colList.begin();
					itCol != pClassRecord->colList.end(); ++itCol)
				{
					const std::string& colTag = itCol->first;
					NFlassRecord::RecordColDesc* pRecordColDesc = itCol->second;

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
	std::string strFileName = strXMLStructPath + "NFrame.sql";

	FILE* iniWriter = fopen(strFileName.c_str(), "w");

	std::string strElementData;
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;
		if (strClassName == "IObject")
		{
			continue;
		}

		strElementData += "CREATE TABLE IF NOT EXISTS " + pClassDta->xStructData.strClassName + "  (";
		strElementData += " `ID` varchar(128) NOT NULL,";
		strElementData += " PRIMARY KEY (`ID`)";
		strElementData += " ) ENGINE=InnoDB DEFAULT CHARSET=utf8; \n";
	}


	//2 fields
	//ALTER TABLE `Buff` ADD `EffectType` bigint(11) DEFAULT '0' COMMENT '影响属性类型(效果类型)  生命，法力(可组合,叠加)';
	// 2.1) property
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;

		for (std::map<std::string, NFlassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& strPropertyName = itProperty->first;
			NFlassProperty* xPropertyData = itProperty->second;

			std::string strType;
			std::string strSave;
			std::string strCache;
			std::string strDesc;
			for (std::map<std::string, std::string>::iterator itDesc = xPropertyData->descList.begin();
				itDesc != xPropertyData->descList.end(); ++itDesc)
			{
				const std::string& strKey = itDesc->first;
				const std::string& strValue = itDesc->second;
				if (strKey=="Type")
				{
					strType = strValue;
				}
				else if (strKey=="Save")
				{
					strSave = strValue;
				} 
				else if (strKey=="Cache")
				{
					strCache = strValue;
				}
				else if (strKey=="Desc")
				{
					strDesc = strValue;
				}
			}

			if (strSave=="1" || strCache=="1")
			{
				std::string strAlter = "\nALTER TABLE `" + strClassName + "` ADD `" + strPropertyName + "`";
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
		for (std::map<std::string, NFlassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& strRecordName = itRecord->first;
			NFlassRecord* xRecordData = itRecord->second;

			std::string strType;
			std::string strSave;
			std::string strCache;
			std::string strDesc;
			for (std::map<std::string, std::string>::iterator itDesc = xRecordData->descList.begin();
				itDesc != xRecordData->descList.end(); ++itDesc)
			{
				const std::string& strKey = itDesc->first;
				const std::string& strValue = itDesc->second;
				if (strKey=="Type")
				{
					strType = strValue;
				}
				else if (strKey=="Save")
				{
					strSave = strValue;
				} 
				else if (strKey=="Cache")
				{
					strCache = strValue;
				}
				else if (strKey=="Desc")
				{
					strDesc = strValue;
				}
			}

			if (strSave=="1" || strCache=="1")
			{
				std::string strAlter = "\nALTER TABLE `" + strClassName + "` ADD `" + strRecordName + "`";
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
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;

		std::cout << "save for struct ---> " << strClassName << std::endl;

		std::string strFileName = strXMLStructPath + strClassName + ".xml";
		FILE* structWriter = fopen(strFileName.c_str(), "w");

		std::string strFileHead = "<?xml version='1.0' encoding='utf-8' ?>\n<XML>\n";
		fwrite(strFileHead.c_str(), strFileHead.length(), 1, structWriter);
		/////////////////////////
		std::string strFilePrpertyBegin = "\t<Propertys>\n";
		fwrite(strFilePrpertyBegin.c_str(), strFilePrpertyBegin.length(), 1, structWriter);

		for (std::map<std::string, NFlassProperty*>::iterator itProperty = pClassDta->xStructData.xPropertyList.begin();
			itProperty != pClassDta->xStructData.xPropertyList.end(); ++itProperty)
		{
			const std::string& strPropertyName = itProperty->first;
			NFlassProperty* xPropertyData = itProperty->second;

			std::string strElementData = "\t\t<Property Id=\"" + strPropertyName + "\" ";
			for (std::map<std::string, std::string>::iterator itDesc = xPropertyData->descList.begin();
				itDesc != xPropertyData->descList.end(); ++itDesc)
			{
				const std::string& strKey = itDesc->first;
				const std::string& strValue = itDesc->second;
				strElementData += strKey + "=\"" + strValue + "\" ";
			}
			strElementData += "/>\n";
			fwrite(strElementData.c_str(), strElementData.length(), 1, structWriter);
		}

		std::string strFilePropertyEnd = "\t</Propertys>\n";
		fwrite(strFilePropertyEnd.c_str(), strFilePropertyEnd.length(), 1, structWriter);
		//////////////////////////////

		std::string strFileRecordBegin = "\t<Records>\n";
		fwrite(strFileRecordBegin.c_str(), strFileRecordBegin.length(), 1, structWriter);

		for (std::map<std::string, NFlassRecord*>::iterator itRecord = pClassDta->xStructData.xRecordList.begin();
			itRecord != pClassDta->xStructData.xRecordList.end(); ++itRecord)
		{
			const std::string& strRecordName = itRecord->first;
			NFlassRecord* xRecordData = itRecord->second;

			//for desc
			std::string strElementData = "\t\t<Record Id=\"" + strRecordName + "\" ";
			for (std::map<std::string, std::string>::iterator itDesc = xRecordData->descList.begin();
				itDesc != xRecordData->descList.end(); ++itDesc)
			{
				const std::string& strKey = itDesc->first;
				const std::string& strValue = itDesc->second;
				strElementData += strKey + "=\"" + strValue + "\"\t ";
			}
			strElementData += ">\n";

			//for col list
			for (int i = 0; i < xRecordData->colList.size(); ++i)
			{
				for (std::map<std::string, NFlassRecord::RecordColDesc*>::iterator itDesc = xRecordData->colList.begin();
					itDesc != xRecordData->colList.end(); ++itDesc)
				{
					const std::string& strKey = itDesc->first;
					const NFlassRecord::RecordColDesc* pRecordColDesc = itDesc->second;

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

		std::string strFilePrpertyEnd = "\t</Records>\n";
		fwrite(strFilePrpertyEnd.c_str(), strFilePrpertyEnd.length(), 1, structWriter);

		/////////////////////////////////
		std::string strFileEnd = "</XML>";
		fwrite(strFileEnd.c_str(), strFileEnd.length(), 1, structWriter);

	}

	return false;
}

bool NFFileProcess::SaveForIni()
{
	ClassData* pBaseObject = mxClassData["IObject"];
	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;

		std::cout << "save for ini ---> " << strClassName << std::endl;

		std::string strFileName = strXMLIniPath + strClassName + ".xml";
		FILE* iniWriter = fopen(strFileName.c_str(), "w");

		std::string strFileHead = "<?xml version='1.0' encoding='utf-8' ?>\n<XML>\n";
		fwrite(strFileHead.c_str(), strFileHead.length(), 1, iniWriter);

		for (std::map<std::string, NFlassElement::ElementData*>::iterator itElement = pClassDta->xIniData.xElementList.begin();
			itElement != pClassDta->xIniData.xElementList.end(); ++itElement)
		{

			const std::string& strElementName = itElement->first;
			NFlassElement::ElementData* pIniData = itElement->second;

			std::string strElementData = "\t<Object Id=\"" + strElementName + "\" ";
			for (std::map<std::string, std::string>::iterator itProperty = pIniData->xPropertyList.begin();
				itProperty != pIniData->xPropertyList.end(); ++itProperty)
			{
				const std::string& strKey = itProperty->first;
				const std::string& strValue = itProperty->second;
				strElementData += strKey + "=\"" + strValue + "\" ";
			}
			strElementData += "/>\n";
			fwrite(strElementData.c_str(), strElementData.length(), 1, iniWriter);
		}

		std::string strFileEnd = "</XML>";
		fwrite(strFileEnd.c_str(), strFileEnd.length(), 1, iniWriter);

	}

	return false;
}

bool NFFileProcess::SaveForLogicClass()
{
	std::string strFileName = strXMLStructPath + "LogicClass.xml";

	FILE* iniWriter = fopen(strFileName.c_str(), "w");

	std::string strFileHead = "<?xml version='1.0' encoding='utf-8' ?>\n<XML>\n";
	fwrite(strFileHead.c_str(), strFileHead.length(), 1, iniWriter);

	ClassData* pBaseObject = mxClassData["IObject"];

	std::string strElementData;
	strElementData += "\t<Class Id=\"" + pBaseObject->xStructData.strClassName + "\"\t";
	strElementData += "Path=\"NFDataCfg/Struct/" + pBaseObject->xStructData.strClassName + ".xml\"\t";
	strElementData += "InstancePath=\"NFDataCfg/Ini/" + pBaseObject->xStructData.strClassName + ".xml\"\t>\n";

	for (std::map<std::string, ClassData*>::iterator it = mxClassData.begin(); it != mxClassData.end(); ++it)
	{
		const std::string& strClassName = it->first;
		ClassData* pClassDta = it->second;
		if (strClassName == "IObject")
		{
			continue;
		}

		strElementData += "\t\t<Class Id=\"" + pClassDta->xStructData.strClassName + "\"\t";
		strElementData += "Path=\"NFDataCfg/Struct/" + pClassDta->xStructData.strClassName + ".xml\"\t";
		strElementData += "InstancePath=\"NFDataCfg/Ini/" + pClassDta->xStructData.strClassName + ".xml\"\t/>\n";

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

std::vector<std::string> NFFileProcess::GetFileListInFolder(std::string folderPath, int depth)
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
