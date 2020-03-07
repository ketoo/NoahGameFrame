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


#ifndef NFI_PROPERTY_H
#define NFI_PROPERTY_H

#include "NFDataList.hpp"
#include "NFList.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"

typedef std::function<int(const NFGUID&, const std::string&, const NFData&, const NFData&)> PROPERTY_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<PROPERTY_EVENT_FUNCTOR> PROPERTY_EVENT_FUNCTOR_PTR;

class _NFExport NFIProperty : public NFMemoryCounter
{
public:
	NFIProperty() : NFMemoryCounter(GET_CLASS_NAME(NFIProperty), 1)
	{
	}

	virtual ~NFIProperty() {}

	virtual void SetValue(const NFData& TData) = 0;
	virtual void SetValue(const NFIProperty* pProperty) = 0;

	virtual bool SetInt(const NFINT64 value) = 0;
	virtual bool SetFloat(const double value) = 0;
	virtual bool SetString(const std::string& value) = 0;
	virtual bool SetObject(const NFGUID& value) = 0;
	virtual bool SetVector2(const NFVector2& value) = 0;
	virtual bool SetVector3(const NFVector3& value) = 0;

	virtual const NFDATA_TYPE GetType() const = 0;
	virtual const bool GeUsed() const = 0;
	virtual const std::string& GetKey() const = 0;
	virtual const bool GetSave() const = 0;
	virtual const bool GetPublic() const = 0;
	virtual const bool GetPrivate() const = 0;
	virtual const bool GetCache() const = 0;
	virtual const bool GetRef() const = 0;
	virtual const bool GetForce() const = 0;
	virtual const bool GetUpload() const = 0;

	virtual void SetSave(bool bSave) = 0;
	virtual void SetPublic(bool bPublic) = 0;
	virtual void SetPrivate(bool bPrivate) = 0;
	virtual void SetCache(bool bCache) = 0;
	virtual void SetRef(bool bRef) = 0;
	virtual void SetForce(bool bRef) = 0;
	virtual void SetUpload(bool bUpload) = 0;

	virtual NFINT64 GetInt() const = 0;
	virtual int GetInt32() const = 0;
	virtual double GetFloat() const = 0;
	virtual const std::string& GetString() const = 0;
	virtual const NFGUID& GetObject() const = 0;
	virtual const NFVector2& GetVector2() const = 0;
	virtual const NFVector3& GetVector3() const = 0;

	virtual const NFData& GetValue() const = 0;
	virtual const NF_SHARE_PTR<NFList<std::string>> GetEmbeddedList() const = 0;
	virtual const NF_SHARE_PTR<NFMapEx<std::string, std::string>> GetEmbeddedMap() const = 0;

	virtual bool Changed() const = 0;

	virtual std::string ToString() = 0;
	virtual bool FromString(const std::string& strData) = 0;
	virtual bool DeSerialization() = 0;

	virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;
};

#endif
