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

#ifndef NFI_RECORD_H
#define NFI_RECORD_H

#include "NFDataList.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"

struct RECORD_EVENT_DATA
{
	enum RecordOptype
	{
		Add = 0,
		Del,//before del
		AfterDel,
		Swap,
		Create,
		Update,
		Cleared,
		Sort,
		Cover,

		UNKNOW,
	};
	RECORD_EVENT_DATA()
	{
		nOpType = UNKNOW;
		nRow = 0;
		nCol = 0;
	}

	RecordOptype nOpType;
	int nRow;
	int nCol;
	std::string strRecordName;
};

typedef std::function<int(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&)> RECORD_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<RECORD_EVENT_FUNCTOR> RECORD_EVENT_FUNCTOR_PTR;

class _NFExport NFIRecord :public NFMemoryCounter
{
public:
    NFIRecord() : NFMemoryCounter(GET_CLASS_NAME(NFIRecord), 1)
	{
	}

    typedef std::vector< NF_SHARE_PTR<NFData> > TRECORDVEC;
    typedef TRECORDVEC::const_iterator TRECORDVECCONSTITER;

    virtual ~NFIRecord() {}

	virtual std::string ToString() = 0;
	virtual bool IsUsed(const int nRow) const  = 0;
    virtual bool SetUsed(const int nRow, const int bUse)  = 0;
	virtual bool PreAllocMemoryForRow(const int nRow) = 0;

    virtual int GetCols() const  = 0;
    virtual int GetRows() const  = 0;

    virtual NFDATA_TYPE GetColType(const int nCol) const = 0;
    virtual const std::string& GetColTag(const int nCol) const = 0;

    
    virtual int AddRow(const int nRow) = 0;
    virtual int AddRow(const int nRow, const NFDataList& var) = 0;

	virtual bool SetRow(const int nRow, const NFDataList& var) = 0;

    virtual bool SetInt(const int nRow, const int nCol, const NFINT64 value) = 0;
    virtual bool SetFloat(const int nRow, const int nCol, const double value) = 0;
    virtual bool SetString(const int nRow, const int nCol, const std::string& value) = 0;
    virtual bool SetObject(const int nRow, const int nCol, const NFGUID& value) = 0;
	virtual bool SetVector2(const int nRow, const int nCol, const NFVector2& value) = 0;
	virtual bool SetVector3(const int nRow, const int nCol, const NFVector3& value) = 0;

    virtual bool SetInt(const int nRow, const std::string& strColTag, const NFINT64 value) = 0;
    virtual bool SetFloat(const int nRow, const std::string& strColTag, const double value) = 0;
    virtual bool SetString(const int nRow, const std::string& strColTag, const std::string& value) = 0;
    virtual bool SetObject(const int nRow, const std::string& strColTag, const NFGUID& value) = 0;
	virtual bool SetVector2(const int nRow, const std::string& strColTag, const NFVector2& value) = 0;
	virtual bool SetVector3(const int nRow, const std::string& strColTag, const NFVector3& value) = 0;

    
    virtual bool QueryRow(const int nRow, NFDataList& varList) = 0;
    virtual bool SwapRowInfo(const int nOriginRow, const int nTargetRow) = 0;

    virtual NFINT64 GetInt(const int nRow, const int nCol) const = 0;
	virtual int GetInt32(const int nRow, const int nCol) const { return (int)GetInt(nRow, nCol); };
	virtual double GetFloat(const int nRow, const int nCol) const = 0;
    virtual const std::string& GetString(const int nRow, const int nCol) const = 0;
    virtual const NFGUID& GetObject(const int nRow, const int nCol) const = 0;
	virtual const NFVector2& GetVector2(const int nRow, const int nCol) const = 0;
	virtual const NFVector3& GetVector3(const int nRow, const int nCol) const = 0;

    virtual NFINT64 GetInt(const int nRow, const std::string& strColTag) const = 0;
	inline int GetInt32(const int nRow, const std::string& strColTag) const { return (int)GetInt(nRow, strColTag); };
	virtual double GetFloat(const int nRow, const std::string& strColTag) const = 0;
    virtual const std::string& GetString(const int nRow, const std::string& strColTag) const = 0;
    virtual const NFGUID& GetObject(const int nRow, const std::string& strColTag) const = 0;
	virtual const NFVector2& GetVector2(const int nRow, const std::string& strColTag) const = 0;
	virtual const NFVector3& GetVector3(const int nRow, const std::string& strColTag) const = 0;

    virtual int FindRowByColValue(const int nCol, const NFData& var, NFDataList& varResult) = 0;
    virtual int FindInt(const int nCol, const NFINT64 value, NFDataList& varResult) = 0;
    virtual int FindFloat(const int nCol, const double value, NFDataList& varResult) = 0;
	virtual int FindString(const int nCol, const std::string& value, NFDataList& varResult) = 0;
    virtual int FindObject(const int nCol, const NFGUID& value, NFDataList& varResult) = 0;
	virtual int FindVector2(const int nCol, const NFVector2& value, NFDataList& varResult) = 0;
	virtual int FindVector3(const int nCol, const NFVector3& value, NFDataList& varResult) = 0;
	
	//return the row and only can used when the col is the primary key
	virtual int FindRowByColValue(const int nCol, const NFData& var) = 0;
	virtual int FindInt(const int nCol, const NFINT64 value) = 0;
	virtual int FindFloat(const int nCol, const double value) = 0;
	virtual int FindString(const int nCol, const std::string& value) = 0;
	virtual int FindObject(const int nCol, const NFGUID& value) = 0;
	virtual int FindVector2(const int nCol, const NFVector2& value) = 0;
	virtual int FindVector3(const int nCol, const NFVector3& value) = 0;

    virtual int FindRowByColValue(const std::string& strColTag, const NFData& var, NFDataList& varResult) = 0;
    virtual int FindInt(const std::string& strColTag, const NFINT64 value, NFDataList& varResult) = 0;
    virtual int FindFloat(const std::string& strColTag, const double value, NFDataList& varResult) = 0;
	virtual int FindString(const std::string& strColTag, const std::string& value, NFDataList& varResult) = 0;
    virtual int FindObject(const std::string& strColTag, const NFGUID& value, NFDataList& varResult) = 0;
	virtual int FindVector2(const std::string& strColTag, const NFVector2& value, NFDataList& varResult) = 0;
	virtual int FindVector3(const std::string& strColTag, const NFVector3& value, NFDataList& varResult) = 0;
 
	//return the row and only can used when the col is the primary key
	virtual int FindRowByColValue(const std::string& strColTag, const NFData& var) = 0;
	virtual int FindInt(const std::string& strColTag, const NFINT64 value) = 0;
	virtual int FindFloat(const std::string& strColTag, const double value) = 0;
	virtual int FindString(const std::string& strColTag, const std::string& value) = 0;
	virtual int FindObject(const std::string& strColTag, const NFGUID& value) = 0;
	virtual int FindVector2(const std::string& strColTag, const NFVector2& value) = 0;
	virtual int FindVector3(const std::string& strColTag, const NFVector3& value) = 0;

    virtual bool Remove(const int nRow) = 0;
    virtual bool Remove(NFDataList& varRows) //need to optimize
    {
        for (int i  = 0; i < varRows.GetCount(); ++i)
        {
            Remove((int)varRows.Int(i));
        }

        return true;
    }

    virtual bool Clear() = 0;

    virtual void AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual const bool GetSave() = 0;
    virtual const bool GetPublic() = 0;
    virtual const bool GetPrivate() = 0;
    virtual const bool GetCache() = 0;
	virtual const bool GetRef() = 0;
	virtual const bool GetForce() = 0;
	virtual const bool GetUpload() = 0;
    virtual const std::string& GetName() const = 0;

    virtual NF_SHARE_PTR<NFDataList> GetInitData() const = 0;
    virtual const NF_SHARE_PTR<NFDataList> GetTag() const = 0;

    virtual void SetSave(const bool bSave) = 0;
    virtual void SetCache(const bool bCache) = 0;
	virtual void SetRef(const bool bRef) = 0;
	virtual void SetForce(const bool bForce) = 0;
	virtual void SetUpload(const bool bUpload) = 0;
    virtual void SetPublic(const bool bPublic) = 0;
    virtual void SetPrivate(const bool bPrivate) = 0;
    virtual void SetName(const std::string& strName) = 0;

    virtual const TRECORDVEC& GetRecordVec() const = 0;
};

#endif
