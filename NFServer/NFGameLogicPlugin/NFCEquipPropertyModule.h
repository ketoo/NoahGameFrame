// -------------------------------------------------------------------------
//    @FileName			:    NFCEquipPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEquipPropertyModule
//
// -------------------------------------------------------------------------

#ifndef NFC_EUIP_PROPERTY_MODULE_H
#define NFC_EUIP_PROPERTY_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEquipModule.h"
#include "NFComm/NFPluginModule/NFIEquipPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCEquipPropertyModule
    : public NFIEquipPropertyModule
{
public:

	NFCEquipPropertyModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCEquipPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	//all data,include base,gem,element
	virtual bool CalEquipProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFIDataList& xDataList);

	//only part of property of equip
	virtual bool CalEquipBaseProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFIDataList& xDataList);
	virtual bool CalEquipRandomProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFIDataList& xDataList);
	virtual bool CalEquipGemProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFIDataList& xDataList);
	virtual bool CalEquipIntensifyProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFIDataList& xDataList);
	virtual bool CalEquipElementProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFIDataList& xDataList);

private:
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
  
};


#endif
