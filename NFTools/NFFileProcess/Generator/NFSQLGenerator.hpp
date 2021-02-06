//
// Created by James on 7/02/21.
//

#ifndef NFSQLGENERATOR_H
#define NFSQLGENERATOR_H

class NFSQLGenerator : public NFIGenerator
{
public:
	NFSQLGenerator()
	{
	}

	virtual bool Generate(const std::map<std::string, ClassData *> &classData) override
	{
//1 class.sql
		std::string fileName = strXMLStructPath + "NFrame.sql";

		FILE* iniWriter = fopen(fileName.c_str(), "w");

		std::string strElementData;
		for (std::map<std::string, ClassData*>::const_iterator it = classData.begin(); it != classData.end(); ++it)
		{
			const std::string& className = it->first;
			ClassData* pClassDta = it->second;

			if (pClassDta->beIncluded || pClassDta->beParted || className == "Include")
			{
				continue;
			}

			strElementData += "CREATE TABLE IF NOT EXISTS " + pClassDta->xStructData.className + "  (";
			strElementData += " `ID` varchar(128) NOT NULL,";
			strElementData += " PRIMARY KEY (`ID`)";
			strElementData += " ) ENGINE=InnoDB DEFAULT CHARSET=utf8; \n";
		}


		//2 fields
		//ALTER TABLE `Buff` ADD `EffectType` bigint(11) DEFAULT '0' COMMENT '影响属性类型(效果类型)  生命，法力(可组合,叠加)';
		// 2.1) property
		for (std::map<std::string, ClassData*>::const_iterator it = classData.begin(); it != classData.end(); ++it)
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
};

#endif //NFSQLGENERATOR_H
