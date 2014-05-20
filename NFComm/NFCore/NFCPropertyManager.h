// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCPropertyManager
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROPERTYMANAGER_H_
#define _NFC_PROPERTYMANAGER_H_

#include "NFDefine.h"
#include "NFMap.h"
#include "NFIValueList.h"
#include "NFIPropertyManager.h"

class NFCPropertyManager : public NFIPropertyManager
{
public:
    NFCPropertyManager(const NFIDENTID& self)
	{
		mSelf = self;
	};

    virtual ~NFCPropertyManager();

    virtual bool RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIValueList& argVar);

    virtual NFIProperty* AddProperty(const NFIDENTID& self, NFIProperty* pProperty);

    virtual NFIProperty* AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const VARIANT_TYPE varType, bool bPublic ,  bool bPrivate ,  bool bSave, int nIndex, const std::string& strScriptFunction);

    virtual bool SetProperty(const std::string& strPropertyName, const NFIValueList::VarData& varData);

    virtual bool SetProperty(const NFIProperty* pProperty);
	
	virtual NFIDENTID Self();

private:
	NFIDENTID mSelf;
};


#endif
