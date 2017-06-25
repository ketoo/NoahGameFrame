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
	for (auto charArr : tmpStrList)
	{
		delete[] charArr;
	}
	tmpStrList.clear();
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



	rapidxml::xml_document<> xmlDoc;

	rapidxml::xml_node<>* pDel = xmlDoc.allocate_node(rapidxml::node_pi, xmlDoc.allocate_string("xml version='1.0' encoding='utf-8'"));
	if (NULL == pDel)
	{
		return;
	}

	xmlDoc.append_node(pDel);

	rapidxml::xml_node<>* root = xmlDoc.allocate_node(rapidxml::node_element, "XML", NULL);
	xmlDoc.append_node(root);

	rapidxml::xml_node<>* classElement = xmlDoc.allocate_node(rapidxml::node_element, "Class", NULL);
	root->append_node(classElement);
	////////////////////////////////////////////////////////////////////////
	classElement->append_attribute(xmlDoc.allocate_attribute("Id", "IObject"));
	classElement->append_attribute(xmlDoc.allocate_attribute("Type", "TYPE_IOBJECT"));

	classElement->append_attribute(xmlDoc.allocate_attribute("Path", NewChar(std::string(strExecutePath + "Struct/Class/IObject.xml"))));
	classElement->append_attribute(xmlDoc.allocate_attribute("InstancePath", NewChar(std::string(strExecutePath + "Ini/NPC/IObject.xml"))));

	classElement->append_attribute(xmlDoc.allocate_attribute("Public", "0"));
	classElement->append_attribute(xmlDoc.allocate_attribute("Desc", "IObject"));
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

		rapidxml::xml_node<>* subClassElement = xmlDoc.allocate_node(rapidxml::node_element, "Class", NULL);
		classElement->append_node(subClassElement);
		subClassElement->append_attribute(xmlDoc.allocate_attribute("Id", NewChar(strFileName)));
		std::string strUpFileName = strFileName;
		transform(strUpFileName.begin(), strUpFileName.end(), strUpFileName.begin(), ::toupper);
		subClassElement->append_attribute(xmlDoc.allocate_attribute("Type", NewChar(std::string("TYPE_" + strUpFileName))));
		subClassElement->append_attribute(xmlDoc.allocate_attribute("Path", NewChar(std::string(strExecutePath + strXMLStructPath + strFileName + ".xml"))));
		subClassElement->append_attribute(xmlDoc.allocate_attribute("InstancePath", NewChar(std::string(strExecutePath + strXMLIniPath + strFileName + ".xml"))));
		subClassElement->append_attribute(xmlDoc.allocate_attribute("Public", "0"));
		subClassElement->append_attribute(xmlDoc.allocate_attribute("Desc", NewChar(strFileName)));
	}



	fwrite("} // !@NFrame\n\n#endif // !NF_PR_NAME_HPP", strlen("} // !@NFrame\n\n#endif // !NF_PR_NAME_HPP"), 1, hppWriter);
	fwrite("}",	strlen("}"), 1, csWriter);

	//auto a = xmlDoc->SaveFile(strLogicClassFile.c_str());
	std::ofstream out(strLogicClassFile.c_str());
	out << xmlDoc;
	out.close();
}

void FileProcess::CreateIniThreadFunc()
{

	auto fileList = GetFileListInFolder(strToolBasePath + strExcelIniPath, 0);
	for (auto fileName : fileList)
	{
		StringReplace(fileName, "\\", "/");
		StringReplace(fileName, "//", "/");

		size_t nLastPoint = fileName.find_last_of(".") + 1;
		size_t nLastSlash = fileName.find_last_of("/") + 1;
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


	rapidxml::xml_document<> structDoc;

	rapidxml::xml_node<>* pDel = structDoc.allocate_node(rapidxml::node_pi, structDoc.allocate_string("xml version='1.0' encoding='utf-8'"));
	if (NULL == pDel)
	{
		return false;
	}

	structDoc.append_node(pDel);

	rapidxml::xml_node<>* root = structDoc.allocate_node(rapidxml::node_element, "XML", NULL);
	structDoc.append_node(root);


	rapidxml::xml_node<>* propertyNodes = structDoc.allocate_node(rapidxml::node_element, "Propertys", NULL);
	root->append_node(propertyNodes);


	rapidxml::xml_node<>* recordNodes = structDoc.allocate_node(rapidxml::node_element, "Records", NULL);
	root->append_node(recordNodes);


	rapidxml::xml_node<>* componentNodes = structDoc.allocate_node(rapidxml::node_element, "Components", NULL);
	root->append_node(componentNodes);


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

				auto propertyNode = structDoc.allocate_node(rapidxml::node_element, "Property", NULL);
				propertyNodes->append_node(propertyNode);
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
					propertyNode->append_attribute(structDoc.allocate_attribute(NewChar(name), NewChar(value)));
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
				auto componentNode = structDoc.allocate_node(rapidxml::node_element, "Component", NULL);
				componentNodes->append_node(componentNode);
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
					componentNode->append_attribute(structDoc.allocate_attribute(NewChar(name), NewChar(value)));
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
						strPublic = cell->value == "TRUE" ? "1" : "0";
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
						strPrivate = cell->value == "TRUE" ? "1" : "0";
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
						strSave = cell->value == "TRUE" ? "1" : "0";
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
						strCache = cell->value == "TRUE" ? "1" : "0";
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
						strUpload = cell->value == "TRUE" ? "1" : "0";
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

				auto recordNode = structDoc.allocate_node(rapidxml::node_element, "Record", NULL);
				recordNodes->append_node(recordNode);

				recordNode->append_attribute(structDoc.allocate_attribute("Id", NewChar(strRecordName)));
				recordNode->append_attribute(structDoc.allocate_attribute("Row", NewChar(strRow)));
				recordNode->append_attribute(structDoc.allocate_attribute("Col", NewChar(strCol)));
				recordNode->append_attribute(structDoc.allocate_attribute("Public", NewChar(strPublic)));
				recordNode->append_attribute(structDoc.allocate_attribute("Private", NewChar(strPrivate)));
				recordNode->append_attribute(structDoc.allocate_attribute("Save", NewChar(strSave)));
				recordNode->append_attribute(structDoc.allocate_attribute("Cache", NewChar(strCache)));
				recordNode->append_attribute(structDoc.allocate_attribute("Upload", NewChar(strUpload)));
				recordNode->append_attribute(structDoc.allocate_attribute("Desc", NewChar(strDesc)));

				strHppRecordInfo = strHppRecordInfo + "\tstatic const std::string& R_" + strRecordName + "(){ static std::string x" + strRecordName + " = \"" + strRecordName + "\";" + " return x" + strRecordName + ";}\n";
				strHppEnumInfo = strHppEnumInfo + "\n\tenum " + strRecordName + "\n\t{\n";

				strJavaRecordInfo = strJavaRecordInfo + "\tpublic static final String R_" + strRecordName + " = \"" + strRecordName + "\";\n";
				strJavaEnumInfo = strJavaEnumInfo + "\n\tpublic enum " + strRecordName + "\n\t{\n";

				strCSRecordInfo = strCSRecordInfo + "\tpublic static readonly String R_" + strRecordName + " = \"" + strRecordName + "\";\n";
				strCSEnumInfo = strCSEnumInfo + "\n\tpublic enum " + strRecordName + "\n\t{\n";

				std::string toWrite = "enum " + strRecordName + "\n{\n";
				fwrite(toWrite.c_str(), toWrite.length(), 1, protoWriter);


				for (int nRecordCol = dim.firstCol; nRecordCol <= dim.lastCol; nRecordCol++)
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

					auto colNode = structDoc.allocate_node(rapidxml::node_element, "Col", NULL);
					recordNode->append_node(colNode);

					colNode->append_attribute(structDoc.allocate_attribute("Type", NewChar(strType)));
					colNode->append_attribute(structDoc.allocate_attribute("Tag", NewChar(strTag)));

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
	size_t nLastPoint = strFilePath.find_last_of(".") + 1;
	size_t nLastSlash = strFilePath.find_last_of("/") + 1;
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

	std::ofstream out(strXMLFile.c_str());
	out << structDoc;
	out.close();
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
	rapidxml::xml_document<> iniDoc;

	rapidxml::xml_node<>* pDel = iniDoc.allocate_node(rapidxml::node_pi, iniDoc.allocate_string("xml version='1.0' encoding='utf-8'"));
	if (NULL == pDel)
	{
		return false;
	}

	iniDoc.append_node(pDel);

	rapidxml::xml_node<>* root = iniDoc.allocate_node(rapidxml::node_element, "XML", NULL);
	iniDoc.append_node(root);

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
						value = valueCell == "TRUE" ? "1" : "0";
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
		auto objectNode = iniDoc.allocate_node(rapidxml::node_element, "Object", NULL);
		root->append_node(objectNode);
		for (auto strColName : vColNames)
		{
			if (strColName == "Id")
			{
				char* chrID = NULL;
				if (objectNode->first_attribute("Id"))
				{
					chrID = objectNode->first_attribute("Id")->value();
				}
				if (!chrID)
				{
					objectNode->append_attribute(iniDoc.allocate_attribute(NewChar(strColName), NewChar(mDataValues[strID + strColName])));
				}
			}
			else
			{
				objectNode->append_attribute(iniDoc.allocate_attribute(NewChar(strColName), NewChar(mDataValues[strID + strColName])));
			}
			nDataCount++;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	size_t nLastPoint = strFile.find_last_of(".") + 1;
	size_t nLastSlash = strFile.find_last_of("/") + 1;
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

	std::ofstream out(strXMLFile.c_str());
	out << iniDoc;
	out.close();
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
	rapidxml::xml_document<> doc;
	char* pData = NULL;
	size_t nDataSize = 0;

	rapidxml::file<> fdoc(strFile.c_str());
	nDataSize = fdoc.size();
	pData = new char[nDataSize + 1];
	strncpy(pData, fdoc.data(), nDataSize);

	pData[nDataSize] = 0;
	doc.parse<0>(pData);

	rapidxml::xml_node<>* root = doc.first_node();
	auto classElement = root->first_node("Class");
	std::string strIObjectPath = classElement->first_attribute("Path")->value();
	auto nodeElement = classElement->first_node();
	if (nodeElement)
	{
		while (true)
		{
			std::string strID = nodeElement->first_attribute("Id")->value();
			std::string sqlToWrite = "CREATE TABLE `" + strID + "` (\n";
			sqlToWrite += "\t`ID` varchar(128) NOT NULL,\n";
			sqlToWrite += "\tPRIMARY KEY (`ID`)\n";
			sqlToWrite += ") ENGINE=InnoDB DEFAULT CHARSET=utf8;\n";
			fwrite(sqlToWrite.c_str(), sqlToWrite.length(), 1, mysqlClassWriter);


			if (!LoadClass(strRelativePath + strIObjectPath, strID))
			{
				return false;
			}


			std::string strPath = nodeElement->first_attribute("Path")->value();
			if (!LoadClass(strRelativePath + strPath, strID))
			{
				return false;
			}

			fwrite("\n", 1, 1, mysqlClassWriter);
			if (nodeElement == classElement->last_node())
			{
				break;
			}
			nodeElement = nodeElement->next_sibling();
		}
	}
	if (NULL != pData)
	{
		delete[] pData;
	}

	return true;
}

bool FileProcess::LoadClass(std::string strFile, std::string strTable)
{
	rapidxml::xml_document<> doc;
	char* pData = NULL;
	size_t nDataSize = 0;

	rapidxml::file<> fdoc(strFile.c_str());
	nDataSize = fdoc.size();
	pData = new char[nDataSize + 1];
	strncpy(pData, fdoc.data(), nDataSize);

	pData[nDataSize] = 0;
	doc.parse<0>(pData);

	rapidxml::xml_node<>* root = doc.first_node("XML");
	auto classElement = root->first_node("Propertys");
	if (classElement)
	{
		auto nodeElement = classElement->first_node("Property");
		if (nodeElement)
		{
			while (true)
			{
				if (!nodeElement || (std::string)(nodeElement->first_attribute("Save")->value()) != "1")
				{
					if (!nodeElement)
					{
						break;
					}
					nodeElement = nodeElement->next_sibling();
					continue;
				}

				std::string strID = nodeElement->first_attribute("Id")->value();

				auto chrDesc = nodeElement->first_attribute("Desc")->value();
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
				std::string strType = nodeElement->first_attribute("Type")->value();

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
				nodeElement = nodeElement->next_sibling();
			}
		}
	}


	auto xRecordsNode = root->first_node("Records");
	if (xRecordsNode)
	{
		auto nodeElement = xRecordsNode->first_node("Record");
		if (nodeElement)
		{
			while (true)
			{
				if (!nodeElement || (std::string)(nodeElement->first_attribute("Save")->value()) != "1")
				{
					if (!nodeElement)
					{
						break;
					}
					nodeElement = nodeElement->next_sibling();
					continue;
				}

				std::string strID = nodeElement->first_attribute("Id")->value();

				auto chrDesc = nodeElement->first_attribute("Desc")->value();
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

				if (nodeElement == classElement->last_node())
				{
					break;
				}
				nodeElement = nodeElement->next_sibling();
			}
		}

	}
	if (NULL != pData)
	{
		delete[]pData;
	}
	return true;
}

char * FileProcess::NewChar(const std::string& str)
{
	char * newChar = new char[str.size()+1];
	strncpy(newChar, str.c_str(), str.size());
	newChar[str.size()] = 0;
	tmpStrList.push_back(newChar);
	return newChar;
}