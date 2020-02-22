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



#ifndef NF_PROPERTY_H
#define NF_PROPERTY_H

#include "NFMap.hpp"
#include "NFList.hpp"
#include "NFDataList.hpp"
#include "NFIProperty.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFProperty : public NFIProperty
{
private:
    NFProperty();

public:
    NFProperty(const NFGUID& self, const std::string& strPropertyName, const NFDATA_TYPE varType);

    virtual ~NFProperty();

    virtual void SetValue(const NFData& TData);
    virtual void SetValue(const NFIProperty* pProperty);

    virtual bool SetInt(const NFINT64 value);
    virtual bool SetFloat(const double value);
    virtual bool SetString(const std::string& value);
    virtual bool SetObject(const NFGUID& value);
	virtual bool SetVector2(const NFVector2& value);
	virtual bool SetVector3(const NFVector3& value);

    virtual const NFDATA_TYPE GetType() const;
    virtual const bool GeUsed() const;
    virtual const std::string& GetKey() const;
    virtual const bool GetSave() const;
    virtual const bool GetPublic() const;
    virtual const bool GetPrivate() const;
    virtual const bool GetCache() const;
	virtual const bool GetRef() const;
	virtual const bool GetForce() const;
	virtual const bool GetUpload() const;

    virtual void SetSave(bool bSave);
    virtual void SetPublic(bool bPublic);
    virtual void SetPrivate(bool bPrivate);
    virtual void SetCache(bool bCache);
	virtual void SetRef(bool bRef);
	virtual void SetForce(bool bRef);
	virtual void SetUpload(bool bUpload);

    virtual NFINT64 GetInt() const;
	virtual int GetInt32() const;
    virtual double GetFloat() const;
    virtual const std::string& GetString() const;
    virtual const NFGUID& GetObject() const;
	virtual const NFVector2& GetVector2() const;
	virtual const NFVector3& GetVector3() const;

    virtual const NFData& GetValue() const;
    virtual const NF_SHARE_PTR<NFList<std::string>> GetEmbeddedList() const;
    virtual const NF_SHARE_PTR<NFMapEx<std::string, std::string>> GetEmbeddedMap() const;

    virtual bool Changed() const;

    virtual std::string ToString();
    virtual void ToMemoryCounterString(std::string& data);
    virtual bool FromString(const std::string& strData);
    virtual bool DeSerialization();

    virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

private:
    int OnEventHandler(const NFData& oldVar, const NFData& newVar);

private:
    typedef std::vector<PROPERTY_EVENT_FUNCTOR_PTR> TPROPERTYCALLBACKEX;
    TPROPERTYCALLBACKEX mtPropertyCallback;

    NFGUID mSelf;
    std::string msPropertyName;
    NFDATA_TYPE eType;

    NF_SHARE_PTR<NFData> mxData;
    NF_SHARE_PTR<NFMapEx<std::string, std::string>> mxEmbeddedMap;
    NF_SHARE_PTR<NFList<std::string>> mxEmbeddedList;

    bool mbPublic;
    bool mbPrivate;
    bool mbSave;
    bool mbCache;
	bool mbRef;
	bool mbForce;
	bool mbUpload;
};

#endif
