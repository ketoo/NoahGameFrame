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

#ifndef NFI_ELEMENTINFO_MODULE_H
#define NFI_ELEMENTINFO_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFIRecordManager.h"

class NFIElementModule
    : public NFIModule
{
public:
    virtual bool Load() = 0;
    virtual bool Save() = 0;
    virtual bool Clear() = 0;

	virtual NFIElementModule* GetThreadElementModule() = 0;

    //special
    virtual bool LoadSceneInfo(const std::string& strFileName, const std::string& strClassName) = 0;

    virtual bool ExistElement(const std::string& strConfigName) = 0;
    virtual bool ExistElement(const std::string& strClassName, const std::string& strConfigName) = 0;

    virtual std::shared_ptr<NFIPropertyManager> GetPropertyManager(const std::string& strConfigName) = 0;
    virtual std::shared_ptr<NFIRecordManager> GetRecordManager(const std::string& strConfigName) = 0;

    virtual NFINT64 GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName) = 0;
	virtual int GetPropertyInt32(const std::string& strConfigName, const std::string& strPropertyName) = 0;
    virtual double GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName) = 0;
    virtual const std::string& GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName) = 0;
	virtual const NFVector2 GetPropertyVector2(const std::string& strConfigName, const std::string& strPropertyName) = 0;
	virtual const NFVector3 GetPropertyVector3(const std::string& strConfigName, const std::string& strPropertyName) = 0;

	virtual const std::vector<std::string> GetListByProperty(const std::string& strClassName, const std::string& strPropertyName, const NFINT64 nValue) = 0;
	virtual const std::vector<std::string> GetListByProperty(const std::string& strClassName, const std::string& strPropertyName, const std::string& nValue) = 0;

};
#endif