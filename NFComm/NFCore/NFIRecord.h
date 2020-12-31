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
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIRecrodWrapperModule.h"
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
		row = 0;
		col = 0;
	}

	RecordOptype nOpType;
	int row;
	int col;
	std::string recordName;
};

typedef std::function<int(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&)> RECORD_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<RECORD_EVENT_FUNCTOR> RECORD_EVENT_FUNCTOR_PTR;

#pragma warning(disable: 4275)
class _NFExport NFIRecord :public NFMemoryCounter
{
#pragma warning(default: 4275)
public:
    NFIRecord() : NFMemoryCounter(GET_CLASS_NAME(NFIRecord), 1)
	{
	}

    typedef std::vector< NF_SHARE_PTR<NFData> > TRECORDVEC;
    typedef TRECORDVEC::const_iterator TRECORDVECCONSTITER;

    virtual ~NFIRecord() {}

	virtual std::string ToString() = 0;
	virtual bool IsUsed(const int row) const  = 0;
    virtual bool SetUsed(const int row, const int bUse)  = 0;
	virtual bool PreAllocMemoryForRow(const int row) = 0;

    virtual int GetCols() const  = 0;
    virtual int GetRows() const  = 0;
	virtual int GetUsedRows() const  = 0;

    virtual NFDATA_TYPE GetColType(const int col) const = 0;
    virtual NFDATA_TYPE GetColType(const string col) const = 0;
    virtual const std::string& GetColTag(const int col) const = 0;

    
    virtual int AddRow(const int row) = 0;
    virtual int AddRow(const int row, const NFDataList& var) = 0;

	virtual bool SetRow(const int row, const NFDataList& var) = 0;

    virtual bool SetInt(const int row, const int col, const NFINT64 value) = 0;
    virtual bool SetFloat(const int row, const int col, const double value) = 0;
    virtual bool SetString(const int row, const int col, const std::string& value) = 0;
    virtual bool SetObject(const int row, const int col, const NFGUID& value) = 0;
	virtual bool SetVector2(const int row, const int col, const NFVector2& value) = 0;
	virtual bool SetVector3(const int row, const int col, const NFVector3& value) = 0;

    virtual bool SetInt(const int row, const std::string& colTag, const NFINT64 value) = 0;
    virtual bool SetFloat(const int row, const std::string& colTag, const double value) = 0;
    virtual bool SetString(const int row, const std::string& colTag, const std::string& value) = 0;
    virtual bool SetObject(const int row, const std::string& colTag, const NFGUID& value) = 0;
	virtual bool SetVector2(const int row, const std::string& colTag, const NFVector2& value) = 0;
	virtual bool SetVector3(const int row, const std::string& colTag, const NFVector3& value) = 0;

    
    virtual bool QueryRow(const int row, NFDataList& varList) = 0;
    virtual bool SwapRowInfo(const int nOriginRow, const int nTargetRow) = 0;

    virtual NFINT64 GetInt(const int row, const int col) const = 0;
	virtual int GetInt32(const int row, const int col) const { return (int)GetInt(row, col); };
	virtual double GetFloat(const int row, const int col) const = 0;
    virtual const std::string& GetString(const int row, const int col) const = 0;
    virtual const NFGUID& GetObject(const int row, const int col) const = 0;
	virtual const NFVector2& GetVector2(const int row, const int col) const = 0;
	virtual const NFVector3& GetVector3(const int row, const int col) const = 0;

    virtual NFINT64 GetInt(const int row, const std::string& colTag) const = 0;
	inline int GetInt32(const int row, const std::string& colTag) const { return (int)GetInt(row, colTag); };
	virtual double GetFloat(const int row, const std::string& colTag) const = 0;
    virtual const std::string& GetString(const int row, const std::string& colTag) const = 0;
    virtual const NFGUID& GetObject(const int row, const std::string& colTag) const = 0;
	virtual const NFVector2& GetVector2(const int row, const std::string& colTag) const = 0;
	virtual const NFVector3& GetVector3(const int row, const std::string& colTag) const = 0;

    virtual int FindRowByColValue(const int col, const NFData& var, NFDataList& varResult) = 0;
    virtual int FindInt(const int col, const NFINT64 value, NFDataList& varResult) = 0;
    virtual int FindFloat(const int col, const double value, NFDataList& varResult) = 0;
	virtual int FindString(const int col, const std::string& value, NFDataList& varResult) = 0;
    virtual int FindObject(const int col, const NFGUID& value, NFDataList& varResult) = 0;
	virtual int FindVector2(const int col, const NFVector2& value, NFDataList& varResult) = 0;
	virtual int FindVector3(const int col, const NFVector3& value, NFDataList& varResult) = 0;
	
	//return the row and only can used when the col is the primary key
	virtual int FindRowByColValue(const int col, const NFData& var) = 0;
	virtual int FindInt(const int col, const NFINT64 value) = 0;
	virtual int FindFloat(const int col, const double value) = 0;
	virtual int FindString(const int col, const std::string& value) = 0;
	virtual int FindObject(const int col, const NFGUID& value) = 0;
	virtual int FindVector2(const int col, const NFVector2& value) = 0;
	virtual int FindVector3(const int col, const NFVector3& value) = 0;

    virtual int FindRowByColValue(const std::string& colTag, const NFData& var, NFDataList& varResult) = 0;
    virtual int FindInt(const std::string& colTag, const NFINT64 value, NFDataList& varResult) = 0;
    virtual int FindFloat(const std::string& colTag, const double value, NFDataList& varResult) = 0;
	virtual int FindString(const std::string& colTag, const std::string& value, NFDataList& varResult) = 0;
    virtual int FindObject(const std::string& colTag, const NFGUID& value, NFDataList& varResult) = 0;
	virtual int FindVector2(const std::string& colTag, const NFVector2& value, NFDataList& varResult) = 0;
	virtual int FindVector3(const std::string& colTag, const NFVector3& value, NFDataList& varResult) = 0;
 
	//return the row and only can used when the col is the primary key
	virtual int FindRowByColValue(const std::string& colTag, const NFData& var) = 0;
	virtual int FindInt(const std::string& colTag, const NFINT64 value) = 0;
	virtual int FindFloat(const std::string& colTag, const double value) = 0;
	virtual int FindString(const std::string& colTag, const std::string& value) = 0;
	virtual int FindObject(const std::string& colTag, const NFGUID& value) = 0;
	virtual int FindVector2(const std::string& colTag, const NFVector2& value) = 0;
	virtual int FindVector3(const std::string& colTag, const NFVector3& value) = 0;

    virtual bool Remove(const int row) = 0;
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
    virtual const bool GetReadOnly() = 0;
    virtual const std::string& GetName() const = 0;
    virtual const std::string& GetClassName() const = 0;

    virtual NF_SHARE_PTR<NFDataList> GetInitData() const = 0;
    virtual const NF_SHARE_PTR<NFDataList> GetTag() const = 0;

    virtual void SetSave(const bool bSave) = 0;
    virtual void SetCache(const bool bCache) = 0;
	virtual void SetRef(const bool bRef) = 0;
	virtual void SetForce(const bool bForce) = 0;
	virtual void SetUpload(const bool bUpload) = 0;
    virtual void SetPublic(const bool bPublic) = 0;
    virtual void SetPrivate(const bool bPrivate) = 0;
    virtual void SetName(const std::string& name) = 0;
    //是否是配置
    virtual void SetReadOnly(const bool bPrivate) = 0;
    //用于读取配置
    virtual void SetClassName(const std::string& name) = 0;
    virtual void SetPluginManger(NFIPluginManager * pluginManger) =0;
    virtual void SetStatic(bool bStatic) =0;
    virtual bool GetStatic() =0;
    virtual int GetValidRowCount() = 0;

    virtual const TRECORDVEC& GetRecordVec() const = 0;
};

#endif
