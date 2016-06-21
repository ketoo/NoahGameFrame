#include "FileProcess.h"
#include <memory>

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
	// 生成proto文件
	CreateProtoFile();

	// 升成Property和Record名字hpp头文件
	CreateNameFile();


	//xml文档
	tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
	if (NULL == xmlDoc)
	{
		return;
	}

	//xml声明
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
	//// 提前把IObject跑一边
	CreateStructXML("../Excel_Struct/IObject.xlsx", "IObject");

	// 遍历Struct文件夹下的excel文件
	auto fileList = GetFileListInFolder(strToolBasePath + strExcelStructPath, 0);

	for (auto fileName : fileList)
	{
		StringReplace(fileName, "\\", "/");
		StringReplace(fileName, "//", "/");
		// 打开excel之后生成的临时文件，略过
		if ((int)(fileName.find("$")) != -1)
		{
			continue;
		}

		// 不是excel文件，默认跳过
		auto strExt = fileName.substr(fileName.find_last_of('.') + 1, fileName.length() - fileName.find_last_of('.') - 1);
		if (strExt != "xlsx")
		{
			continue;
		}

		// 是IObject.xlsx跳过
		auto strFileName = fileName.substr(fileName.find_last_of('/') + 1, fileName.find_last_of('.') - fileName.find_last_of('/') - 1);
		if (strFileName == "IObject")
		{
			continue;
		}

		// 单个excel文件转为xml
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

	//// name文件结束

	fwrite("} // !@NFrame\n\n#endif // !NF_PR_NAME_HPP",
		sizeof("} // !@NFrame\n\n#endif // !NF_PR_NAME_HPP"), 1, hppWriter);
	fwrite("}",
		sizeof("}"), 1, csWriter);
	xmlDoc->SetBOM(false);
	auto a = xmlDoc->SaveFile(strLogicClassFile.c_str());//保存文件 参数：路径

	//xmlDoc->Print();//打印
	delete xmlDoc;
}

void FileProcess::CreateIniThreadFunc()
{
	// 遍历Ini文件夹下的excel文件
	auto fileList = GetFileListInFolder(strToolBasePath + strExcelIniPath, 0);
	for (auto fileName : fileList)
	{
		StringReplace(fileName, "\\", "/");
		StringReplace(fileName, "//", "/");

		int nLastPoint = fileName.find_last_of(".") + 1;
		int nLastSlash = fileName.find_last_of("/") + 1;
		std::string strFileName = fileName.substr(nLastSlash, nLastPoint - nLastSlash - 1);
		std::string strFileExt = fileName.substr(nLastPoint, fileName.length() - nLastPoint);

		// 打开excel之后生成的临时文件，略过
		if ((int)(fileName.find("$")) != -1)
		{
			continue;
		}

		// 不是excel文件，默认跳过
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
	// 打开excel
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

	// 开始创建xml
	tinyxml2::XMLDocument* structDoc = new tinyxml2::XMLDocument();
	if (NULL == structDoc)
	{
		return false;
	}
	//xml声明
	tinyxml2::XMLDeclaration *pDel = structDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	if (NULL == pDel)
	{
		return false;
	}

	structDoc->LinkEndChild(pDel);

	// 写入XML root标签
	tinyxml2::XMLElement* root = structDoc->NewElement("XML");
	structDoc->LinkEndChild(root);

	// 写入Propertys标签
	tinyxml2::XMLElement* propertyNodes = structDoc->NewElement("Propertys");
	root->LinkEndChild(propertyNodes);

	// 写入Records标签
	tinyxml2::XMLElement* recordNodes = structDoc->NewElement("Records");
	root->LinkEndChild(recordNodes);

	// 写入components的处理
	tinyxml2::XMLElement* componentNodes = structDoc->NewElement("Components");
	root->LinkEndChild(componentNodes);

	// 读取excel中每一个sheet
	std::vector<MiniExcelReader::Sheet>& sheets = x->sheets();
	for (MiniExcelReader::Sheet& sh : sheets)
	{
		std::string strSheetName = sh.getName();
		const MiniExcelReader::Range& dim = sh.getDimension();

		std::vector<std::string> colNames;
		for (int c = dim.firstCol; c <= dim.lastCol; c++)
		{
			MiniExcelReader::Cell* cell = sh.getCell(dim.firstRow, c);
			if (cell)
			{
				colNames.push_back(cell->value);
			}
		}
		std::string strUpperSheetName(strSheetName);
		transform(strUpperSheetName.begin(), strUpperSheetName.end(), strUpperSheetName.begin(), ::tolower);
		if (strUpperSheetName == "property")
		{
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
				auto propertyNode = structDoc->NewElement("Property");
				propertyNodes->LinkEndChild(propertyNode);
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
		else if (strUpperSheetName == "component")
		{
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
			for (int RecordNumber = 0; RecordNumber < (int)(dim.lastRow / 2); RecordNumber++)
			{
				auto recordNode = structDoc->NewElement("Record");
				recordNodes->LinkEndChild(recordNode);

				std::string strRecordName = "";

				// 从11个开始就是Record中具体的Col了
				int nSetColNum = 0;

				for (int c = dim.firstCol; c <= nRecordStart; c++)
				{
					std::string  name = colNames[c - 1];
					std::string  value = "";

					MiniExcelReader::Cell* cell = sh.getCell(dim.firstRow + RecordNumber * 2 + 1, c);
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

						recordNode->SetAttribute(name.c_str(), value.c_str());
						if (name == "Col")
						{
							nSetColNum = atoi(value.c_str());
						}

						if (name == "Id")
						{
							strRecordName = value;
						}
					}
				}

				strHppRecordInfo = strHppRecordInfo + "\tstatic const std::string& R_" + strRecordName + "(){ static std::string x" + strRecordName + " = \"" + strRecordName + "\";" + " return x" + strRecordName + ";}\n";
				strHppEnumInfo = strHppEnumInfo + "\n\tenum " + strRecordName + "\n\t{\n";

				strJavaRecordInfo = strJavaRecordInfo + "\tpublic static final String R_" + strRecordName + " = \"" + strRecordName + "\";\n";
				strJavaEnumInfo = strJavaEnumInfo + "\n\tpublic enum " + strRecordName + "\n\t{\n";

				strCSRecordInfo = strCSRecordInfo + "\tpublic static readonly String R_" + strRecordName + " = \"" + strRecordName + "\";\n";
				strCSEnumInfo = strCSEnumInfo + "\n\tpublic enum " + strRecordName + "\n\t{\n";

				std::string toWrite = "enum " + strRecordName + "\n{\n";
				fwrite(toWrite.c_str(), toWrite.length(), 1, protoWriter);

				int nExcelColsNum = nSetColNum;
				int nRealColNum = 0;

				nSetColNum = dim.lastCol - nRecordStart;

				for (int c = nRecordStart + 1; c <= nRecordStart + nSetColNum; c++)
				{
					std::string name = "";

					auto cll = sh.getCell(dim.firstRow + RecordNumber * 2, c);
					if (cll)
					{
						name = cll->value;
					}
					else
					{
						break;
					}

					nRealColNum++;

					std::string value = "";

					MiniExcelReader::Cell* cell = sh.getCell(dim.firstRow + RecordNumber * 2 + 1, c);
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

						auto colNode = structDoc->NewElement("Col");
						recordNode->LinkEndChild(colNode);

						colNode->SetAttribute("Type", value.c_str());
						colNode->SetAttribute("Tag", name.c_str());

						toWrite = "\t" + name + "\t\t= " + std::to_string(c - nRecordStart - 1) + "; // " + name + " -- " + value + " \n";
						fwrite(toWrite.c_str(), toWrite.length(), 1, protoWriter);

						strHppEnumInfo += "\t\t" + strRecordName + "_" + name + "\t\t= " + std::to_string(c - 11 - 1) + ", // " + name + " -- " + value + "\n";
						strJavaEnumInfo += "\t\t" + name + "\t\t= " + std::to_string(c - nRecordStart - 1) + ", // " + name + " -- " + value + "\n";
						strCSEnumInfo += "\t\t" + name + "\t\t= " + std::to_string(c - nRecordStart - 1) + ", // " + name + " -- " + value + "\n";
					}
				}

				if (nExcelColsNum != nRealColNum)
				{
					printf("This Excel[%s]'s format is something wrong, Record[%s] field \"col\"==%d not equal the real cols==%d!\n", strFile.c_str(), strRecordName.c_str(), nExcelColsNum, nRealColNum);
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
	// 保存文件
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
	// 打开excel
	MiniExcelReader::ExcelFile* x = new MiniExcelReader::ExcelFile();
	if (!x->open(strFile.c_str()))
	{
		printf("can't open %s\n", strFile.c_str());
		return false;
	}
	////////////////////////////////////////////////////////////////////////////
	// 开始创建xml
	tinyxml2::XMLDocument* iniDoc = new tinyxml2::XMLDocument();
	if (NULL == iniDoc)
	{
		return false;
	}
	//xml声明
	tinyxml2::XMLDeclaration *pDel = iniDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	if (NULL == pDel)
	{
		return false;
	}

	iniDoc->LinkEndChild(pDel);

	// 写入XML root标签
	tinyxml2::XMLElement* root = iniDoc->NewElement("XML");
	iniDoc->LinkEndChild(root);

	// 读取excel中每一个sheet
	std::vector<MiniExcelReader::Sheet>& sheets = x->sheets();
	for (MiniExcelReader::Sheet& sh : sheets)
	{
		const MiniExcelReader::Range& dim = sh.getDimension();

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
			auto objectNode = iniDoc->NewElement("Object");
			root->LinkEndChild(objectNode);
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
				}
				objectNode->SetAttribute(name.c_str(), value.c_str());
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
	// 保存文件
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

			// 读取父节点内容
			if (!LoadClass(strRelativePath + strIObjectPath, strID))
			{
				return false;
			}

			// 读取自己节点内容
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
				if (IsTextUTF8(chrDesc, descLength))
				{
					if (descLength > 0)
					{
						char* chrArrDesc = new char[descLength];
						UTF8ToGBK((char*)chrDesc, chrArrDesc, descLength);
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
				if (IsTextUTF8(chrDesc, descLength))
				{
					if (descLength > 0)
					{
						char* chrArrDesc = new char[descLength];
						UTF8ToGBK((char*)chrDesc, chrArrDesc, descLength);
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
