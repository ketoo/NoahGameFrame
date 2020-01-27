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


#ifndef NFI_PROPERTYMANAGER_H
#define NFI_PROPERTYMANAGER_H

#include "NFMap.hpp"
#include "NFIProperty.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFIPropertyManager
    : public NFMapEx<std::string, NFIProperty>
{
public:
    virtual ~NFIPropertyManager() {}

    virtual bool RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFGUID& self, NF_SHARE_PTR<NFIProperty> pProperty) = 0;

    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFGUID& self, const std::string& strPropertyName, const NFDATA_TYPE varType) = 0;

    virtual const NFGUID& Self() = 0;

	virtual std::string ToString() = 0;
    //////////////////////////////////////////////////////////////////////////
    virtual bool SetProperty(const std::string& strPropertyName, const NFData& TData) = 0;

    virtual bool SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue) = 0;
    virtual bool SetPropertyFloat(const std::string& strPropertyName, const double dwValue) = 0;
    virtual bool SetPropertyString(const std::string& strPropertyName, const std::string& strValue) = 0;
    virtual bool SetPropertyObject(const std::string& strPropertyName, const NFGUID& obj) = 0;
	virtual bool SetPropertyVector2(const std::string& strPropertyName, const NFVector2& value) = 0;
	virtual bool SetPropertyVector3(const std::string& strPropertyName, const NFVector3& value) = 0;

    virtual NFINT64 GetPropertyInt(const std::string& strPropertyName) = 0;
	virtual int GetPropertyInt32(const std::string& strPropertyName) = 0;
    virtual double GetPropertyFloat(const std::string& strPropertyName) = 0;
    virtual const std::string& GetPropertyString(const std::string& strPropertyName) = 0;
    virtual const NFGUID& GetPropertyObject(const std::string& strPropertyName) = 0;
	virtual const NFVector2& GetPropertyVector2(const std::string& strPropertyName) = 0;
	virtual const NFVector3& GetPropertyVector3(const std::string& strPropertyName) = 0;

    //////////////////////////////////////////////////////////////////////////

private:
};

#endif
