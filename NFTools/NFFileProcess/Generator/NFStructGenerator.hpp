//
// Created by James on 7/02/21.
//

#ifndef NFSTRUCTGENERATOR_H
#define NFSTRUCTGENERATOR_H

#if NF_PLATFORM == NF_PLATFORM_WIN
#include<direct.h>
#endif

class NFStructGenerator : public NFIGenerator
{
public:
	NFStructGenerator()
	{
	}

	virtual bool Generate(const std::map<std::string, ClassData *> &classData) override
	{

		ClassData* pBaseObject = classData.at("IObject");

		for (std::map<std::string, ClassData*>::const_iterator it = classData.begin(); it != classData.end(); ++it)
		{
			if (it->second->beParted)
			{
				continue;
			}

			const std::string& className = it->first;
			ClassData* pClassDta = it->second;

			std::cout << "save for struct ---> " << className << std::endl;
			std::string path = pClassDta->filePath;
			NFFileProcessHelp::StringReplace(path, strExcelIniPath, "");
			std::string fileName = strXMLStructPath + path + ".xml";

			FILE* structWriter = fopen(fileName.c_str(), "w+");
			if (structWriter == nullptr)
			{
				std::string folder = pClassDta->fileFolder;
				NFFileProcessHelp::StringReplace(folder, strExcelIniPath, "");
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
					auto includeClass = classData.at(item);

					std::string path = includeClass->filePath;
					NFFileProcessHelp::StringReplace(path, strExcelIniPath, "");
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
};

#endif //NFSTRUCTGENERATOR_H
