//
// Created by James on 7/02/21.
//

#ifndef NFINIGENERATOR_H
#define NFINIGENERATOR_H

class NFIniGenerator : public NFIGenerator
{
public:
	NFIniGenerator()
	{
	}

	virtual bool Generate(const std::map<std::string, ClassData *> &classData) override
	{

		ClassData* pBaseObject = classData.at("IObject");
		for (std::map<std::string, ClassData*>::const_iterator it = classData.begin(); it != classData.end(); ++it)
		{
			const std::string& className = it->first;
			ClassData* pClassDta = it->second;

			if (pClassDta->beIncluded || pClassDta->beParted || className == "Include")
			{
				continue;
			}

			std::cout << "save for ini ---> " << className << std::endl;

			std::string path = pClassDta->filePath;
			NFFileProcessHelp::StringReplace(path, strExcelIniPath, "");
			std::string fileName = strXMLIniPath + path + ".xml";

			FILE* iniWriter = fopen(fileName.c_str(), "w+");
			if (iniWriter == nullptr)
			{
				std::string folder = pClassDta->fileFolder;
				NFFileProcessHelp::StringReplace(folder, strExcelIniPath, "");
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
};

#endif //NFINIGENERATOR_H
