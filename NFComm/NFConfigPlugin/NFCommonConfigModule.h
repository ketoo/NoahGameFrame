/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#ifndef NF_COMMONCONFIG_MODULE_H
#define NF_COMMONCONFIG_MODULE_H

#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCommonConfigModule
    : public NFICommonConfigModule
{
public:
public:
	class CAttributeList : public NFMapEx<std::string, std::string>
	{
	public:
		const int GetInt(const std::string& strName)
		{
			NF_SHARE_PTR<std::string> pStr = GetElement(strName);
			if (pStr && !pStr->empty())
			{
				int nData = 0;
				NF_StrTo(*pStr, nData);
				return nData;
			}

			return 0;
		}

		const std::string& GetString(const std::string& strName)
		{
			NF_SHARE_PTR<std::string> pStr = GetElement(strName);
			if (pStr)
			{
				return *pStr;
			}

			return NULL_STR;
		}
	};

	struct CStructInfo : public NFMapEx<std::string, CAttributeList>
	{
		CAttributeList  mmStructAttribute;
	};

public:
    NFCommonConfigModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCommonConfigModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual bool ClearConfig();
	virtual bool LoadConfig(const std::string& strFile);

	virtual const int GetFieldInt(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute);
	virtual const int GetFieldInt(const std::string& strStructName, const std::string& strSDKAttribute);
	
	virtual const std::string& GetFieldString(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute);
	virtual const std::string& GetFieldString(const std::string& strStructName,const std::string& strSDKAttribute);
	
	virtual std::vector<std::string> GetSubKeyList(const std::string&strStructName);
	virtual std::vector<std::string> GetFieldList(const std::string&strStructName);

private:
	NFMapEx<std::string, CStructInfo> mmData; //strStructName<-->CStructInfo
};


#endif
