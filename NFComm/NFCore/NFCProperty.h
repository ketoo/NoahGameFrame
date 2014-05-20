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
#include "NFCValueList.h"
#include "NFIProperty.h"

class NFCProperty : public NFIProperty
{
public:
    NFCProperty();

    NFCProperty(const NFIDENTID& self, const std::string& strPropertyName, const VARIANT_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, int nIndex, const std::string& strScriptFunction);

    virtual ~NFCProperty();

    virtual void SetValue(const NFIValueList::VarData& varData);
    virtual void SetValue(const NFIProperty* pProperty);

    virtual bool SetInt(int value);
    virtual bool SetFloat(float value);
    virtual bool SetDouble(double value);
    virtual bool SetString(const std::string& value);
    virtual bool SetObject(const NFIDENTID& value);
    virtual bool SetPointer(void* value);

    virtual const VARIANT_TYPE GetType() const;
    virtual const bool GeUsed() const;
    virtual const std::string& GetKey() const;
    virtual const bool GetSave() const;
    virtual const bool GetPublic() const;
    virtual const bool GetPrivate() const;
    virtual const int GetIndex() const;
    virtual const std::string& GetScriptFunction() const;

    virtual void SetSave(bool bSave);
    virtual void SetPublic(bool bPublic);
    virtual void SetPrivate(bool bPrivate);
    virtual void SetScriptFunction(const std::string& strScriptFunction);

    virtual int QueryInt() const;
    virtual float QueryFloat() const;
    virtual double QueryDouble() const;
    virtual const std::string& QueryString() const;
    virtual NFIDENTID QueryObject() const;
    virtual void* QueryPointer() const;

    virtual bool Changed() const;

    //virtual void RegisterCallback(PROPERTY_EVENT_FUNC cb, const NFIValueList& argVar);
    virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIValueList& argVar);


protected:
    virtual NFIValueList::VarData GetValue() const;

    //int OnEventHandler(const NFIValueList& oldVar, const NFIValueList& newVar);
    int OnEventHandler(const NFIValueList& oldVar, const NFIValueList& newVar);
private:
//     typedef std::map<PROPERTY_EVENT_FUNC, NFIValueList::VarData> TPROPERTYCALLBACK;
//     TPROPERTYCALLBACK mtPropertyCallback;

    typedef std::vector<PROPERTY_EVENT_FUNCTOR_PTR> TPROPERTYCALLBACKEX;
    TPROPERTYCALLBACKEX mtPropertyCallback;

    NFIDENTID mSelf;
    std::string msPropertyName;//可以想办法与基本类型共用
    VARIANT_TYPE eType;//只有在不存在指针的时候才用这个判断类型--为节约内存
    //NFCValueList    mVarProperty;
    //NFIValueList::VarData* m_pVarData;
    std::shared_ptr<NFIValueList::VarData> m_pVarData;

    bool mbPublic;
    bool mbPrivate;
    bool mbSave;
    NFINT16 mnIndex;//属性编号，用于同步时识别
    //std::string msScriptFunction;
};

#endif
