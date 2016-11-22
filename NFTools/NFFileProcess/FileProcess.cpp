#include "FileProcess.h"
#include <memory>
#include "Utf8ToGbk.h"

FileProcess::FileProcess()
{
	strLogicClassFile = "../Struct/LogicClass.xml";
	mysqlWriter = fopen(strMySQLFile.c_str(), "w");
	mysqlClassWriter = fopen(strMySQLClassFile.c_str(), "w");
	protoWriter = fopen(strProtoFile.c_str(), "w");
	hppWriter = fopen(strHPPFile.c_str(), "w");
	javaWriter = fopen(strJavaFile.c_str(), "w");
	csWriter = fopen(strCSFile.c_str(), "w");
}

FileProcess::~FileProcess()
{
	fclose(mysqlWriter);
	fclose(mysqlClassWriter);
	fclose(protoWriter);
	fclose(hppWriter);
	fclose(javaWriter);
	fclose(csWriter);
}

void FileProcess::CreateProtoFile()
{
	std::string toWrite = "package NFrame;\n";
	if ((protoWriter) == NULL)
		return;
	fwrite(toWrite.c_str(), toWrite.length(), 1, protoWriter);
}

void FileProcess::CreateNameFile()
{
	time_t timeval;
	timeval = time(NULL);
	std::string strHPPHead
		= "// -------------------------------------------------------------------------\n";
	strHPPHead = strHPPHead
		+ "//    @FileName         :    NFProtocolDefine.hpp\n"
		+ "//    @Author           :    NFrame Studio\n"
		+ "//    @Module           :    NFProtocolDefine\n"
		+ "// -------------------------------------------------------------------------\n\n"
		+ "#ifndef NF_PR_NAME_HPP\n"
		+ "#define NF_PR_NAME_HPP\n\n"
		+ "#include <string>\n"
		+ "namespace NFrame\n{\n";

	/////////////////////////////////////////////////////
	fwrite(strHPPHead.c_str(), strHPPHead.length(), 1, hppWriter);
	/////////////////////////////////////////////////////

	std::string strJavaHead
		= "// -------------------------------------------------------------------------\n";
	strJavaHead = strJavaHead
		+ "//    @FileName         :    NFProtocolDefine.java\n"
		+ "//    @Author           :    NFrame Studio\n"
		+ "//    @Module           :    NFProtocolDefine\n"
		+ "// -------------------------------------------------------------------------\n\n"
		+ "package nframe;\n";

	fwrite(strJavaHead.c_str(), strJavaHead.length(), 1, javaWriter);
	/////////////////////////////////////////////////////

	std::string strCSHead = "// -------------------------------------------------------------------------\n";
	strCSHead = strCSHead
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

	fwrite(strCSHead.c_str(), strCSHead.length(), 1, csWriter);
	/////////////////////////////////////////////////////
}

void FileProcess::OnCreateXMLFile()
{
	//////////////////////////////////////////////////////////////////////
	CreateStructThreadFunc();
	CreateIniThreadFunc();
}

void FileProcess::CreateStructThreadFunc()
{
	
	CreateProtoFile();

	
	CreateNameFile();


	
	tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
	if (NULL == xmlDoc)
	{
		return;
	}

	
	tinyxml2::XMLDeclaration *pDel = xmlDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	if (NULL == pDel)
	{
		return;
	}

	xmlDoc->LinkEndChild(pDel);

	tinyxml2::XMLElement* root = xmlDoc->NewElement("XML");
	xmlDoc->LinkEndChild(root);

	tinyxml2::XMLElement* classElement = xmlDoc->NewElement("Class");
	root->LinkEndChild(classElement);
	////////////////////////////////////////////////////////////////////////
	classElement->SetAttribute("Id", "IObject");
	classElement->SetAttribute("Type", "TYPE_IOBJECT");

	classElement->SetAttribute("Path", (strExecutePath + "Struct/Class/IObject.xml").c_str());
	classElement->SetAttribute("InstancePath", (strExecutePath + "Ini/NPC/IObject.xml").c_str());

	classElement->SetAttribute("Public", "0");
	classElement->SetAttribute("Desc", "IObject");
	////////////////////////////////////////////////////////////////////////
	
	CreateStructXML("../Excel_Ini/IObject.xlsx", "IObject");

	
	auto fileList = GetFileListInFolder(strToolBasePath + strExcelIniPath, 0);

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

		
		if (!CreateStructXML(fileName.c_str(), strFileName.c_str()))
		{
			std::cout << "Create " + fileName + " failed!" << std::endl;
			return;
		}

		tinyxml2::XMLElement* subClassElement = xmlDoc->NewElement("Class");
		classElement->LinkEndChild(subClassElement);

		subClassElement->SetAttribute("Id", strFileName.c_str());
		std::string strUpFileName = strFileName;
		transform(strUpFileName.begin(), strUpFileName.end(), strUpFileName.begin(), ::toupper);
		subClassElement->SetAttribute("Type", ("TYPE_" + strUpFileName).c_str());

		subClassElement->SetAttribute("Path", (strExecutePath + strXMLStructPath + strFileName + ".xml").c_str());
		subClassElement->SetAttribute("InstancePath", (strExecutePath + strXMLIniPath + strFileName + ".xml").c_str());

		subClassElement->SetAttribute("Public", "0");
		subClassElement->SetAttribute("Desc", strFileName.c_str());
	}

	

	fwrite("} // !@NFrame\n\n#endif // !NF_PR_NAME_HPP",
		sizeof("} // !@NFrame\n\n#endif // !NF_PR_NAME_HPP"), 1, hppWriter);
	fwrite("}",
		sizeof("}"), 1, csWriter);
	xmlDoc->SetBOM(false);
	auto a = xmlDoc->SaveFile(strLogicClassFile.c_str());

	//xmlDoc->Print();
	delete xmlDoc;
}

void FileProcess::CreateIniThreadFunc()
{
	
	auto fileList = GetFileListInFolder(strToolBasePath + strExcelIniPath, 0);
	for (auto fileName : fileList)
	{
		StringReplace(fileName, "\\", "/");
		StringReplace(fileName, "//", "/");

		int nLastPoint = fileName.find_last_of(".") + 1;
		int nLastSlash = fileName.find_last_of("/") + 1;
		std::string strFileName = fileName.substr(nLastSlash, nLastPoint - nLastSlash - 1);
		std::string strFileExt = fileName.substr(nLastPoint, fileName.length() - nLastPoint);

		
		if ((int)(fileName.find("$")) != -1)
		{
			continue;
		}

		
		auto strExt = fileName.substr(fileName.find_last_of('.') + 1, fileName.length() - fileName.find_last_of('.') - 1);
		if (strExt != "xlsx")
		{
			continue;
		}

		if (!CreateIniXML(fileName))
		{
			std::cout << "Create " + fileName + " failed!" << std::endl;
		}
	}
}

bool FileProcess::CreateStructXML(std::string strFile, std::string strFileName)
{
	std::cout << strFile << std::endl;
	
	MiniExcelReader::ExcelFile* x = new MiniExcelReader::ExcelFile();
	if (!x->open(strFile.c_str()))
	{
		printf("can't open %s\n", strFile.c_str());
		return false;
	}

	// PropertyName
	// cpp
	std::string strHPPPropertyInfo = "";
	std::string strHppRecordInfo = "";
	std::string strHppEnumInfo = "";

	strHPPPropertyInfo = strHPPPropertyInfo + "class " + strFileName + "\n{\npublic:\n";
	strHPPPropertyInfo = strHPPPropertyInfo + "\t//Class name\n\tstatic const std::string& ThisName(){ static std::string x" + strFileName + " = \"" + strFileName + "\";" + " return x" + strFileName + "; }\n" + "\t// IObject\n" + strHppIObjectInfo + "\t// Property\n";

	// java
	std::string strJavaPropertyInfo = "";
	std::string strJavaRecordInfo = "";
	std::string strJavaEnumInfo = "";

	strJavaPropertyInfo = strJavaPropertyInfo + "public class " + strFileName + " {\n";
	strJavaPropertyInfo = strJavaPropertyInfo + "\t//Class name\n\tpublic static final String ThisName = \"" + strFileName + "\";\n\t// IObject\n" + strJavaIObjectInfo + "\t// Property\n";

	// C#
	std::string strCSPropertyInfo = "";
	std::string strCSRecordInfo = "";
	std::string strCSEnumInfo = "";

	strCSPropertyInfo = strCSPropertyInfo + "public class " + strFileName + "\n{\n";
	strCSPropertyInfo = strCSPropertyInfo + "\t//Class name\n\tpublic static readonly string ThisName = \"" + strFileName + "\";\n\t// IObject\n" + strCSIObjectInfo + "\t// Property\n";

	
	tinyxml2::XMLDocument* structDoc = new tinyxml2::XMLDocument();
	if (NULL == structDoc)
	{
		return false;
	}
	
	tinyxml2::XMLDeclaration *pDel = structDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	if (NULL == pDel)
	{
		return false;
	}

	structDoc->LinkEndChild(pDel);

	
	tinyxml2::XMLElement* root = structDoc->NewElement("XML");
	structDoc->LinkEndChild(root);

	
	tinyxml2::XMLElement* propertyNodes = structDoc->NewElement("Propertys");
	root->LinkEndChild(propertyNodes);

	
	tinyxml2::XMLElement* recordNodes = structDoc->NewElement("Records");
	root->LinkEndChild(recordNodes);

	
	tinyxml2::XMLElement* componentNodes = structDoc->NewElement("Components");
	root->LinkEndChild(componentNodes);

	
	std::vector<MiniExcelReader::Sheet>& sheets = x->sheets();
	for (MiniExcelReader::Sheet& sh : sheets)
	{
		
		int nTitleLine = 9;
		std::string strSheetName = sh.getName();

		const MiniExcelReader::Range& dim = sh.getDimension();

		std::string strUpperSheetName = strSheetName.substr(0, 8);
		transform(strUpperSheetName.begin(), strUpperSheetName.end(), strUpperSheetName.begin(), ::tolower);
		if (strUpperSheetName == "property")
		{
			std::vector<std::string> colNames;
			for (int r = dim.firstRow; r <= dim.firstRow + nTitleLine - 1; r++)
			{
				MiniExcelReader::Cell* cell = sh.getCell(r, dim.firstCol);
				if (cell)
				{
					colNames.push_back(cell->value);
				}
			}

			for (int c = dim.firstCol + 1; c <= dim.lastCol; c++)
			{
				std::string testValue = "";
				MiniExcelReader::Cell* cell = sh.getCell(dim.firstRow, c);
				if (cell)
				{
					testValue = cell->value;
				}
				if (testValue == "")
				{
					continue;
				}

				auto propertyNode = structDoc->NewElement("Property");
				propertyNodes->LinkEndChild(propertyNode);
				std::string strType = "";
				for (int r = dim.firstRow; r <= dim.firstRow + nTitleLine - 1; r++)
				{
					std::string name = colNames[r - 1];
					std::string value = "";
					MiniExcelReader::Cell* cell = sh.getCell(r, c);
					if (cell)
					{
						std::string valueCell = cell->value;
						transform(valueCell.begin(), valueCell.end(), valueCell.begin(), ::toupper);
						if (valueCell == "TRUE" || valueCell == "FALSE")
						{
							value = valueCell == "TRUE" ? 1 : 0;
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
					propertyNode->SetAttribute(name.c_str(), value.c_str());
				}

				strHPPPropertyInfo = strHPPPropertyInfo + "\tstatic const std::string& " + testValue + "(){ static std::string x" + testValue + " = \"" + testValue + "\";" + " return x" + testValue + "; } // " + strType + "\n";
				strJavaPropertyInfo = strJavaPropertyInfo + "\tpublic static final String " + testValue + " = \"" + testValue + "\"; // " + strType + "\n";
				strCSPropertyInfo = strCSPropertyInfo + "\tpublic static readonly String " + testValue + " = \"" + testValue + "\"; // " + strType + "\n";

				if (strFileName == "IObject")
				{
					strHppIObjectInfo += "\tstatic const std::string& " + testValue + "(){ static std::string x" + testValue + " = \"" + testValue + "\";" + " return x" + testValue + "; } // " + strType + "\n";
					strJavaIObjectInfo += "\tpublic static final String " + testValue + " = \"" + testValue + "\"; // " + strType + "\n";
					strCSIObjectInfo += "\tpublic static readonly String " + testValue + " = \"" + testValue + "\"; // " + strType + "\n";
				}
			}
		}
		else if (strUpperSheetName == "componen")
		{
			std::vector<std::string> colNames;
			for (int c = dim.firstCol; c <= dim.lastCol; c++)
			{
				MiniExcelReader::Cell* cell = sh.getCell(dim.firstRow, c);
				if (cell)
				{
					colNames.push_back(cell->value);
				}
			}
			for (int r = dim.firstRow + 1; r <= dim.lastRow; r++)
			{
				std::string testValue = "";
				MiniExcelReader::Cell* cell = sh.getCell(r, dim.firstCol);
				if (cell)
				{
					testValue = cell->value;
				}
				if (testValue == "")
				{
					continue;
				}
				auto componentNode = structDoc->NewElement("Component");
				componentNodes->LinkEndChild(componentNode);
				std::string strType = "";
				for (int c = dim.firstCol; c <= dim.lastCol; c++)
				{
					std::string name = colNames[c - 1];
					std::string value = "";
					MiniExcelReader::Cell* cell = sh.getCell(r, c);
					if (cell)
					{
						std::string valueCell = cell->value;
						transform(valueCell.begin(), valueCell.end(), valueCell.begin(), ::toupper);
						if (valueCell == "TRUE" || valueCell == "FALSE")
						{
							value = valueCell == "TRUE" ? 1 : 0;
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
					componentNode->SetAttribute(name.c_str(), value.c_str());
				}
			}
		}
		else
		{
			const int nRecordLineCount = 11;
			const int nRowsCount = dim.lastRow - dim.firstRow + 1;
			const int nRecordCount = nRowsCount / nRecordLineCount;

			if (nRowsCount != nRecordCount * nRecordLineCount)
			{
				printf("This Excel[%s]'s Record is something wrong, Sheet[%s] Total Rows is %d lines, Not 10*N\n", strFile.c_str(), strSheetName.c_str(), nRowsCount);
				printf("Generate faild!\n");
				printf("Press [Enter] key to exit!\n");
				std::cin.ignore();
				exit(1);
			}

			for (int nCurrentRecord = 1; nCurrentRecord <= nRecordCount; nCurrentRecord++)
			{
				std::string strRecordName = "";
				std::string strRow = "";
				std::string strCol = "";
				std::string strPublic = "";
				std::string strPrivate = "";
				std::string strSave = "";
				std::string strCache = "";
				std::string strUpload = "";
				std::string strDesc = "";

				int nRelativeRow = 1;
				MiniExcelReader::Cell* cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					strRecordName = cell->value;
				}
				cell = nullptr;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					strRow = cell->value;
				}
				cell = nullptr;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					strCol = cell->value;
				}
				cell = nullptr;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					if (cell->value == "TRUE" || cell->value == "FALSE")
					{
						strPublic = cell->value == "TRUE" ? 1 : 0;
					}
					else
					{
						strPublic = cell->value;
					}
				}
				cell = nullptr;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					if (cell->value == "TRUE" || cell->value == "FALSE")
					{
						strPrivate = cell->value == "TRUE" ? 1 : 0;
					}
					else
					{
						strPrivate = cell->value;
					}
				}
				cell = nullptr;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					if (cell->value == "TRUE" || cell->value == "FALSE")
					{
						strSave = cell->value == "TRUE" ? 1 : 0;
					}
					else
					{
						strSave = cell->value;
					}
				}
				cell = nullptr;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					if (cell->value == "TRUE" || cell->value == "FALSE")
					{
						strCache = cell->value == "TRUE" ? 1 : 0;
					}
					else
					{
						strCache = cell->value;
					}
				}
				cell = nullptr;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					if (cell->value == "TRUE" || cell->value == "FALSE")
					{
						strUpload = cell->value == "TRUE" ? 1 : 0;
					}
					else
					{
						strUpload = cell->value;
					}
				}
				cell = nullptr;
				int nTagRow = nRelativeRow++;
				int nTypeRow = nRelativeRow++;
				cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nRelativeRow++, 2);
				if (cell)
				{
					strDesc = cell->value;
				}
				cell = nullptr;

				const int nExcelCols = atoi(strCol.c_str());
				int nRealCols = 0;

				auto recordNode = structDoc->NewElement("Record");
				recordNodes->LinkEndChild(recordNode);

				recordNode->SetAttribute("Id", strRecordName.c_str());
				recordNode->SetAttribute("Row", strRow.c_str());
				recordNode->SetAttribute("Col", strCol.c_str());
				recordNode->SetAttribute("Public", strPublic.c_str());
				recordNode->SetAttribute("Private", strPrivate.c_str());
				recordNode->SetAttribute("Save", strSave.c_str());
				recordNode->SetAttribute("Cache", strCache.c_str());
				recordNode->SetAttribute("Upload", strUpload.c_str());
				recordNode->SetAttribute("Desc", strDesc.c_str());

				strHppRecordInfo = strHppRecordInfo + "\tstatic const std::string& R_" + strRecordName + "(){ static std::string x" + strRecordName + " = \"" + strRecordName + "\";" + " return x" + strRecordName + ";}\n";
				strHppEnumInfo = strHppEnumInfo + "\n\tenum " + strRecordName + "\n\t{\n";

				strJavaRecordInfo = strJavaRecordInfo + "\tpublic static final String R_" + strRecordName + " = \"" + strRecordName + "\";\n";
				strJavaEnumInfo = strJavaEnumInfo + "\n\tpublic enum " + strRecordName + "\n\t{\n";

				strCSRecordInfo = strCSRecordInfo + "\tpublic static readonly String R_" + strRecordName + " = \"" + strRecordName + "\";\n";
				strCSEnumInfo = strCSEnumInfo + "\n\tpublic enum " + strRecordName + "\n\t{\n";

				std::string toWrite = "enum " + strRecordName + "\n{\n";
				fwrite(toWrite.c_str(), toWrite.length(), 1, protoWriter);


				for (int nRecordCol = dim.firstCol;nRecordCol <= dim.lastCol;nRecordCol++)
				{
					std::string strType = "";
					std::string strTag = "";
					cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nTagRow, nRecordCol);
					if (cell)
					{
						strTag = cell->value;
					}
					else
					{
						break;
					}
					cell = nullptr;
					cell = sh.getCell((nCurrentRecord - 1) * nRecordLineCount + nTypeRow, nRecordCol);
					if (cell)
					{
						strType = cell->value;
					}
					else
					{
						break;
					}

					cell = nullptr;

					auto colNode = structDoc->NewElement("Col");
					recordNode->LinkEndChild(colNode);

					colNode->SetAttribute("Type", strType.c_str());
					colNode->SetAttribute("Tag", strTag.c_str());

					toWrite = "\t" + strTag + "\t\t= " + std::to_string(nRecordCol - 1) + "; // " + strTag + " -- " + strType + "\n";
					fwrite(toWrite.c_str(), toWrite.length(), 1, protoWriter);

					strHppEnumInfo += "\t\t" + strRecordName + "_" + strTag + "\t\t= " + std::to_string(nRecordCol - 1) + ", // " + strTag + " -- " + strType + "\n";
					strJavaEnumInfo += "\t\t" + strTag + "\t\t= " + std::to_string(nRecordCol - 1) + ", // " + strTag + " -- " + strType + "\n";
					strCSEnumInfo += "\t\t" + strTag + "\t\t= " + std::to_string(nRecordCol - 1) + ", // " + strTag + " -- " + strType + "\n";

					nRealCols++;
				}

				if (nExcelCols != nRealCols)
				{
					printf("This Excel[%s]'s format is something wrong, Record[%s] field \"col\"==%d not equal the real cols==%d!\n", strFile.c_str(), strRecordName.c_str(), nExcelCols, nRealCols);
					printf("Press [Enter] key to exit!");
					std::cin.ignore();
					exit(1);
				}

				fwrite("}\n", 2, 1, protoWriter);

				strHppEnumInfo += "\n\t};\n";
				strJavaEnumInfo += "\n\t};\n";
				strCSEnumInfo += "\n\t};\n";
			}
		}
	}
	// cpp
	strHPPPropertyInfo += "\t// Record\n" + strHppRecordInfo + strHppEnumInfo + "\n};\n\n";
	fwrite(strHPPPropertyInfo.c_str(), strHPPPropertyInfo.length(), 1, hppWriter);

	// java
	strJavaPropertyInfo += "\t// Record\n" + strJavaRecordInfo + strJavaEnumInfo + "\n}\n\n";
	fwrite(strJavaPropertyInfo.c_str(), strJavaPropertyInfo.length(), 1, javaWriter);

	// C#
	strCSPropertyInfo += "\t// Record\n" + strCSRecordInfo + strCSEnumInfo + "\n}\n\n";
	fwrite(strCSPropertyInfo.c_str(), strCSPropertyInfo.length(), 1, csWriter);

	////////////////////////////////////////////////////////////////////////////
	
	std::string strFilePath(strFile);
	int nLastPoint = strFilePath.find_last_of(".") + 1;
	int nLastSlash = strFilePath.find_last_of("/") + 1;
	std::string strFileExt = strFilePath.substr(nLastPoint, strFilePath.length() - nLastPoint);

	std::string strXMLFile = strToolBasePath + strXMLStructPath + strFileName;
	if (nCipher > 0)
	{
		strXMLFile += ".NF";
	}
	else
	{
		strXMLFile += ".xml";
	}
	structDoc->SetBOM(false);
	structDoc->SaveFile(strXMLFile.c_str());
	delete structDoc;
	delete x;
	return true;
}

bool FileProcess::CreateIniXML(std::string strFile)
{
	std::cout << strFile << std::endl;
	
	MiniExcelReader::ExcelFile* x = new MiniExcelReader::ExcelFile();
	if (!x->open(strFile.c_str()))
	{
		printf("can't open %s\n", strFile.c_str());
		return false;
	}
	////////////////////////////////////////////////////////////////////////////
	
	tinyxml2::XMLDocument* iniDoc = new tinyxml2::XMLDocument();
	if (NULL == iniDoc)
	{
		return false;
	}
	
	tinyxml2::XMLDeclaration *pDel = iniDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	if (NULL == pDel)
	{
		return false;
	}

	iniDoc->LinkEndChild(pDel);

	
	tinyxml2::XMLElement* root = iniDoc->NewElement("XML");
	iniDoc->LinkEndChild(root);

	
	std::vector<MiniExcelReader::Sheet>& sheets = x->sheets();
	std::vector<std::string> vColNames;
	std::vector<std::string> vDataIDs;
	std::map<std::string, std::string> mDataValues;
	int nCurrentCol = 0;

	for (MiniExcelReader::Sheet& sh : sheets)
	{
		
		int nDataLine = 10;

		std::string strSheetName = sh.getName();

		std::string strUpperSheetName = strSheetName.substr(0, 8);
		transform(strUpperSheetName.begin(), strUpperSheetName.end(), strUpperSheetName.begin(), ::tolower);

		if (strUpperSheetName != "property")
		{
			continue;
		}

		const MiniExcelReader::Range& dim = sh.getDimension();

		for (int c = dim.firstCol; c <= dim.lastCol; c++)
		{
			MiniExcelReader::Cell* cell = sh.getCell(dim.firstRow, c);
			if (cell)
			{
				vColNames.push_back(cell->value);
			}
		}

		if (vDataIDs.size() <= 0)
		{
			for (int r = dim.firstRow + nDataLine - 1; r <= dim.lastRow; r++)
			{
				MiniExcelReader::Cell* cell = sh.getCell(r, dim.firstCol);
				if (cell)
				{
					if (cell->value.length() > 0)
					{
						vDataIDs.push_back(cell->value);
					}
				}
			}
		}

		for (int r = dim.firstRow + nDataLine - 1; r <= vDataIDs.size() + nDataLine - 1; r++)
		{
			std::string testValue = "";
			MiniExcelReader::Cell* cell = sh.getCell(r, dim.firstCol);

			for (int c = dim.firstCol; c <= dim.lastCol; c++)
			{
				std::string name = vColNames[c - 1 + nCurrentCol];
				std::string value = "";
				MiniExcelReader::Cell* cell = sh.getCell(r, c);
				std::string vType = sh.getCell(dim.firstRow + 1, c)->value;
				if (cell)
				{
					std::string valueCell = cell->value;
					transform(valueCell.begin(), valueCell.end(), valueCell.begin(), ::toupper);
					if (valueCell == "TRUE" || valueCell == "FALSE")
					{
						value = valueCell == "TRUE" ? 1 : 0;
					}
					else
					{
						value = cell->value;
						if (value.size() <= 0)
						{
							if (vType == "int" || vType == "float")
							{
								value = "0";
							}
							else
							{
								value = "";
							}
						}
					}
				}
				else
				{
					if (vType == "int" || vType == "float")
					{
						value = "0";
					}
					else
					{
						value = "";
					}
				}
				//check the field is utf8, then convert it into gbk.
				auto descLength = value.size();
				if (bConvertIntoUTF8 && IsTextUTF8(value.c_str(), descLength))
				{
					if (descLength > 0)
					{
						char* chrArrDesc = new char[descLength];
						Utf8ToGbk((char*)value.c_str(), chrArrDesc);
						value = chrArrDesc;
						delete[] chrArrDesc;
					}
				}
				mDataValues.insert(std::pair<string, string>(vDataIDs[r - nDataLine] + name, value));
			}
		}
		nCurrentCol += dim.lastCol;
	}

	int nDataCount = 0;
	if (strFile.find("NPC") > 0 && strFile.find("NPC") < strFile.size())
	{
		int a = 0;
	}
	for (auto strID : vDataIDs)
	{
		auto objectNode = iniDoc->NewElement("Object");
		root->LinkEndChild(objectNode);
		for (auto strColName : vColNames)
		{
			if (strColName == "Id")
			{
				const char* chrID = objectNode->Attribute("Id");
				if (!chrID)
				{
					objectNode->SetAttribute(strColName.c_str(), mDataValues[strID + strColName].c_str());
				}
			}
			else
			{
				objectNode->SetAttribute(strColName.c_str(), mDataValues[strID + strColName].c_str());
			}
			nDataCount++;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	
	int nLastPoint = strFile.find_last_of(".") + 1;
	int nLastSlash = strFile.find_last_of("/") + 1;
	std::string strFileName = strFile.substr(nLastSlash, nLastPoint - nLastSlash - 1);
	std::string strFileExt = strFile.substr(nLastPoint, strFile.length() - nLastPoint);

	std::string strXMLFile = strToolBasePath + strXMLIniPath + strFileName;
	if (nCipher > 0)
	{
		strXMLFile += ".NF";
	}
	else
	{
		strXMLFile += ".xml";
	}

	iniDoc->SetBOM(false);
	iniDoc->SaveFile(strXMLFile.c_str());
	delete iniDoc;
	delete x;
	return true;
}

void FileProcess::OnCreateMysqlFile()
{
	if (!LoadLogicClass(strLogicClassFile))
	{
		std::cout << "Generate MySQL file failed, please check files!" << std::endl;
	}
}

bool FileProcess::LoadLogicClass(std::string strFile)
{
	////////////////////////////////////////////////////
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	if (NULL == doc)
	{
		return false;
	}
	doc->LoadFile(strFile.c_str());

	tinyxml2::XMLElement* root = doc->RootElement();
	auto classElement = root->FirstChildElement("Class");
	std::string strIObjectPath = classElement->Attribute("Path");
	auto nodeElement = classElement->FirstChildElement();
	if (nodeElement)
	{
		while (true)
		{
			std::string strID = nodeElement->Attribute("Id");
			std::string sqlToWrite = "CREATE TABLE `" + strID + "` (\n";
			sqlToWrite += "\t`ID` varchar(128) NOT NULL,\n";
			sqlToWrite += "\tPRIMARY KEY (`ID`)\n";
			sqlToWrite += ") ENGINE=InnoDB DEFAULT CHARSET=utf8;\n";
			fwrite(sqlToWrite.c_str(), sqlToWrite.length(), 1, mysqlClassWriter);

			
			if (!LoadClass(strRelativePath + strIObjectPath, strID))
			{
				return false;
			}

			
			std::string strPath = nodeElement->Attribute("Path");
			if (!LoadClass(strRelativePath + strPath, strID))
			{
				return false;
			}

			fwrite("\n", 1, 1, mysqlClassWriter);
			if (nodeElement == classElement->LastChildElement())
			{
				break;
			}
			nodeElement++;
		}
	}

	delete doc;
	return true;
}

bool FileProcess::LoadClass(std::string strFile, std::string strTable)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	if (NULL == doc)
	{
		return false;
	}
	doc->LoadFile(strFile.c_str());
	auto ff = doc->Value();
	tinyxml2::XMLElement* root = doc->RootElement();
	auto classElement = root->FirstChildElement("Propertys");
	if (classElement)
	{
		auto nodeElement = classElement->FirstChildElement("Property");
		if (nodeElement)
		{
			while (true)
			{
				if (!nodeElement || (std::string)(nodeElement->Attribute("Save")) != "1")
				{
					if (!nodeElement)
					{
						break;
					}
					nodeElement = nodeElement->NextSiblingElement();
					continue;
				}

				std::string strID = nodeElement->Attribute("Id");

				auto chrDesc = nodeElement->Attribute("Desc");
				std::string strDesc = chrDesc;
				auto descLength = strlen(chrDesc);
				if (bConvertIntoUTF8 && IsTextUTF8(chrDesc, descLength))
				{
					if (descLength > 0)
					{
						char* chrArrDesc = new char[descLength];
						Utf8ToGbk((char*)chrDesc, chrArrDesc);
						strDesc = chrArrDesc;
						delete[] chrArrDesc;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				std::string strType = nodeElement->Attribute("Type");

				std::string toWrite = "";
				if (strType == "string")
				{
					toWrite = "ALTER TABLE `" + strTable + "` ADD `" + strID + "` varchar(128) DEFAULT '' COMMENT '" + strDesc + "';";
				}
				else if (strType == "int")
				{
					toWrite = "ALTER TABLE `" + strTable + "` ADD `" + strID + "` bigint(11) DEFAULT '0' COMMENT '" + strDesc + "';";
				}
				else if (strType == "object")
				{
					toWrite = "ALTER TABLE `" + strTable + "` ADD `" + strID + "` varchar(128) DEFAULT '' COMMENT '" + strDesc + "';";
				}
				else if (strType == "float")
				{
					toWrite = "ALTER TABLE `" + strTable + "` ADD `" + strID + "` float(11,3) DEFAULT '0' COMMENT '" + strDesc + "';";
				}
				else
				{
					toWrite = "ALTER TABLE `" + strTable + "` ADD `" + strID + "` varchar(128) DEFAULT '' COMMENT '" + strDesc + "';";
				}
				toWrite += "\n";
				fwrite(toWrite.c_str(), toWrite.length(), 1, mysqlWriter);

				if (!nodeElement)
				{
					break;
				}
				nodeElement = nodeElement->NextSiblingElement();
			}
		}
	}


	auto xRecordsNode = root->FirstChildElement("Records");
	if (xRecordsNode)
	{
		auto nodeElement = xRecordsNode->FirstChildElement("Record");
		if (nodeElement)
		{
			while (true)
			{
				if (!nodeElement || (std::string)(nodeElement->Attribute("Save")) != "1")
				{
					if (!nodeElement)
					{
						break;
					}
					nodeElement = nodeElement->NextSiblingElement();
					continue;
				}

				std::string strID = nodeElement->Attribute("Id");

				auto chrDesc = nodeElement->Attribute("Desc");
				std::string strDesc = chrDesc;
				auto descLength = strlen(chrDesc);
				if (bConvertIntoUTF8 && IsTextUTF8(chrDesc, descLength))
				{
					if (descLength > 0)
					{
						char* chrArrDesc = new char[descLength];
						Utf8ToGbk((char*)chrDesc, chrArrDesc);
						strDesc = chrArrDesc;
						delete[] chrArrDesc;
					}
				}

				std::string toWrite = "ALTER TABLE `" + strTable + "` ADD `" + strID + "` BLOB COMMENT '" + strDesc + "';";
				toWrite += "\n";
				fwrite(toWrite.c_str(), toWrite.length(), 1, mysqlWriter);

				if (nodeElement == classElement->LastChildElement())
				{
					break;
				}
				nodeElement = nodeElement->NextSiblingElement();
			}
		}

	}
	delete doc;
	return true;
}
