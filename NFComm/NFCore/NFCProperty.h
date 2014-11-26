// -------------------------------------------------------------------------
//    @FileName         :    NFCProperty.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCProperty
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROPERTY_H_
#define _NFC_PROPERTY_H_

#include "NFDefine.h"
#include "NFMap.h"
#include "NFCDataList.h"
#include "NFIProperty.h"

class NFCProperty : public NFIProperty
{
public:
    NFCProperty();

    NFCProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex, const std::string& strScriptFunction);

    virtual ~NFCProperty();

    virtual void SetValue(const NFIDataList::TData& TData);
    virtual void SetValue(const NFIProperty* pProperty);

    virtual bool SetInt(const NFINT64 value);
    virtual bool SetFloat(const float value);
    virtual bool SetDouble(const double value);
    virtual bool SetString(const std::string& value);
    virtual bool SetObject(const NFIDENTID& value);
    virtual bool SetPointer(const void* value);

    virtual const TDATA_TYPE GetType() const;
    virtual const bool GeUsed() const;
    virtual const std::string& GetKey() const;
    virtual const bool GetSave() const;
    virtual const bool GetView() const;
    virtual const bool GetPublic() const;
    virtual const bool GetPrivate() const;
    virtual const int GetIndex() const;
    virtual const std::string& GetRelationValue() const;

    virtual void SetSave(bool bSave);
    virtual void SetView(bool bView);
    virtual void SetPublic(bool bPublic);
    virtual void SetPrivate(bool bPrivate);
    virtual void SetScriptFunction(const std::string& strScriptFunction);

    virtual NFINT64 GetInt() const;
    virtual float GetFloat() const;
    virtual double GetDouble() const;
    virtual const std::string& GetString() const;
    virtual NFIDENTID GetObject() const;
    virtual void* GetPointer() const;

    virtual bool Changed() const;

    virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIDataList& argVar);

protected:
    virtual NFIDataList::TData GetValue() const;

    int OnEventHandler(const NFIDataList& oldVar, const NFIDataList& newVar);
private:
    typedef std::vector<PROPERTY_EVENT_FUNCTOR_PTR> TPROPERTYCALLBACKEX;
    TPROPERTYCALLBACKEX mtPropertyCallback;

    NFIDENTID mSelf;
    std::string msPropertyName;//������취��������͹���
    TDATA_TYPE eType;//ֻ���ڲ�����ָ���ʱ���������ж�����--Ϊ��Լ�ڴ�
    //NFCDataList    mVarProperty;
    //NFIDataList::TData* m_pTData;
    std::shared_ptr<NFIDataList::TData> m_pTData;

    bool mbPublic;
    bool mbPrivate;
    bool mbSave;
    bool mbView;
    NFINT16 mnIndex;//���Ա�ţ�����ͬ��ʱʶ��
    //std::string msScriptFunction;
};

#endif
