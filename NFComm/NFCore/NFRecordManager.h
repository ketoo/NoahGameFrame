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



#ifndef NF_RECORDMANAGER_H
#define NF_RECORDMANAGER_H

#include <map>
#include <list>
#include <string>
#include <vector>
#include "NFRecord.h"
#include "NFIRecordManager.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFRecordManager : public NFIRecordManager
{
public:
    NFRecordManager(const NFGUID& self)
    {
        mSelf = self;
    }

    virtual ~NFRecordManager();

    virtual NF_SHARE_PTR<NFIRecord> AddRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFDataList>& TData, const NF_SHARE_PTR<NFDataList>& tagData, const int nRows);

    virtual const NFGUID& Self();

	virtual std::string ToString();
    //////////////////////////////////////////////////////////////////////////

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue);
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj);
	virtual bool SetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& obj);
	virtual bool SetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& obj);

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value);
	virtual bool SetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value);
	virtual bool SetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value);

    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol);
    virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const int nCol);
    virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol);
	virtual const NFVector2& GetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol);
	virtual const NFVector3& GetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol);

    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const NFVector2& GetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const NFVector3& GetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag);
    //////////////////////////////////////////////////////////////////////////
private:
    NFGUID mSelf;

};

#endif
