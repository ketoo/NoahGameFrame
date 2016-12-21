// -------------------------------------------------------------------------
//    @FileName         :    NFCDataList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCDataList
//
// -------------------------------------------------------------------------

#ifndef NFC_DATALIST_H
#define NFC_DATALIST_H

#include "NFIDataList.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFCDataList: public NFIDataList
{
public:
    NFCDataList();

    NFCDataList(const std::string& str, const std::string& strSplit);

    NFCDataList(const NFCDataList& src);
    NFCDataList(const NFIDataList& src);

    virtual ~NFCDataList();

    
    virtual bool Append(const NFIDataList& src);
    virtual bool Append(const NFIDataList& src, const int start, const int count);

    
    virtual bool Append(const NFIDataList::TData& TData);

    
    virtual void Clear();

    
    virtual bool IsEmpty() const;

    
    virtual int GetCount() const;

    
    virtual bool Concat(const NFIDataList& src);

    //////////////////////////////////////////////////////////////////////////
    const NF_SHARE_PTR<NFIDataList::TData> GetStack(const int index) const;

    
    virtual TDATA_TYPE Type(const int index) const;
    int TypeInt(const int index) const;
    
    virtual bool TypeEx(const  int nType, ...) const;

    virtual bool Split(const std::string& strData, const std::string& strSplit);

    virtual bool Add(const NFINT64 value);
    virtual bool Add(const double value);
    virtual bool Add(const std::string& value);
    virtual bool Add(const NFGUID& value);
	virtual bool Add(const NFVector2& value);
	virtual bool Add(const NFVector3& value);

    virtual bool Set(const int index, const NFINT64 value);
    virtual bool Set(const int index, const double value);
	virtual bool Set(const int index, const std::string& value);
    virtual bool Set(const int index, const NFGUID& value);
	virtual bool Set(const int index, const NFVector2& value);
	virtual bool Set(const int index, const NFVector3& value);

    
    virtual NFINT64 Int(const int index) const;
    virtual double Float(const int index) const;
    virtual std::string StringValEx(const int index) const;
    virtual const std::string& String(const int index) const;
    virtual const NFGUID& Object(const int index) const;
	virtual const NFVector2& Vector2(const int index) const;
	virtual const NFVector3& Vector3(const int index) const;

    virtual bool ToString(std::string& str, const std::string& strSplit) const;


protected:
    void AddStatck();
    void InnerAppendEx(const NFIDataList& src, const int start, const int end);
    bool ValidIndex(int index) const;
};

#endif