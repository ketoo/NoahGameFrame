// -------------------------------------------------------------------------
//    @FileName         :    NFCValueList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCValueList
//
// -------------------------------------------------------------------------

#ifndef _NFC_VALUELIST_H_
#define _NFC_VALUELIST_H_

#include "NFIdentID.h"
#include "NFIValueList.h"

class NFCValueList: public NFIValueList
{
public:
    NFCValueList();

    NFCValueList(const char* str, const char* strSplit);

    NFCValueList(const NFIValueList& src);

    virtual ~NFCValueList();

    NFCValueList& operator=(const NFIValueList& src);

    // ���
    virtual bool Append(const NFIValueList& src, int start, int count);

    // ���
    virtual bool Append(const NFIValueList::VarData& varData);

    // ���
    virtual void Clear();

    // �Ƿ�Ϊ��
    virtual bool IsEmpty() const;

    // ��������
    virtual int GetCount() const;

    // �ϲ�
    virtual bool Concat(const NFIValueList& src);

    //////////////////////////////////////////////////////////////////////////


    // ��������
    virtual VARIANT_TYPE Type(const int index) const;
    //�������ͼ��
    virtual bool TypeEx(const  int nType, ...) const;

    virtual bool Split(const char* strData, const char* strSplit);

    virtual bool AddInt(const int value);

    virtual bool AddFloat(const float value);

    virtual bool AddDouble(const double value);

    virtual bool AddString(const char* value);

    virtual bool AddObject(const NFIDENTID& value);

    virtual bool AddPointer(const void* value);

    virtual bool SetInt(const int index, const int value);

    virtual bool SetFloat(const int index, const float value);

    virtual bool SetDouble(const int index, const double value);

    virtual bool SetString(const int index, const char* value);

    virtual bool SetObject(const int index, const NFIDENTID& value);

    virtual bool SetPointer(const int index, const void* value);

    // �������
    virtual int IntVal(const int index) const;
    virtual float FloatVal(const int index) const;
    virtual double DoubleVal(const int index) const;
    virtual std::string StringValEx(const int index, const bool bForce) const;
    virtual bool ToString(OUT std::string& str, const char* strSplit);
    virtual const std::string& StringVal(const int index) const;
    virtual NFIDENTID ObjectVal(const int index) const;
    virtual void* PointerVal(const int index) const;

protected:
    void InnerAppendEx(const NFIValueList& src, const int start, const int end);
};

#endif
