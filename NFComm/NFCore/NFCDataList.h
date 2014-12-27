// -------------------------------------------------------------------------
//    @FileName         :    NFCDataList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCDataList
//
// -------------------------------------------------------------------------

#ifndef _NFC_DATALIST_H_
#define _NFC_DATALIST_H_

#include "NFIdentID.h"
#include "NFIDataList.h"

class NFCDataList: public NFIDataList
{
public:
    NFCDataList();

    NFCDataList(const char* str, const char* strSplit);

    NFCDataList(const NFCDataList& src);
    NFCDataList(const NFIDataList& src);

    virtual ~NFCDataList();

    NFCDataList& operator=(const NFCDataList& src);
    NFCDataList& operator=(const NFIDataList& src);

    // ���
    virtual bool Append(const NFIDataList& src, int start, int count);

    // ���
    virtual bool Append(const NFIDataList::TData& TData);

    // ���
    virtual void Clear();

    // �Ƿ�Ϊ��
    virtual bool IsEmpty() const;

    // ��������
    virtual int GetCount() const;

    // �ϲ�
    virtual bool Concat(const NFIDataList& src);

    //////////////////////////////////////////////////////////////////////////


    // ��������
    virtual TDATA_TYPE Type(const int index) const;
    //�������ͼ��
    virtual bool TypeEx(const  int nType, ...) const;

    virtual bool Split(const char* strData, const char* strSplit);

    virtual bool Add(const NFINT64 value);

    virtual bool Add(const float value);

    virtual bool Add(const double value);

    virtual bool Add(const char* value);

    virtual bool Add(const NFIDENTID& value);

    virtual bool Add(const void* value);

    virtual bool Set(const int index, const NFINT64 value);

    virtual bool Set(const int index, const float value);

    virtual bool Set(const int index, const double value);

    virtual bool Set(const int index, const char* value);

    virtual bool Set(const int index, const NFIDENTID& value);

    virtual bool Set(const int index, const void* value);

    // �������
    virtual NFINT64 Int(const int index) const;
    virtual float Float(const int index) const;
    virtual double Double(const int index) const;
    virtual std::string StringValEx(const int index, const bool bForce) const;
    virtual bool ToString(std::string& str, const char* strSplit);
    virtual const std::string& String(const int index) const;
    virtual NFIDENTID Object(const int index) const;
    virtual void* Pointer(const int index) const;

protected:
    void InnerAppendEx(const NFIDataList& src, const int start, const int end);
};

#endif
