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

	virtual bool CalEquipProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFIDataList& xDataList);

private:
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
  
};


#endif
