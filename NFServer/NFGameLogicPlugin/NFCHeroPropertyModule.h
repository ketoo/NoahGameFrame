// -------------------------------------------------------------------------
//    @FileName      :    NFCHeroPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCHeroPropertyModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HERO_PROPERTY_MODULE_MODULE_H
#define NFC_HERO_PROPERTY_MODULE_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIHeroPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIEquipPropertyModule.h"

class NFCHeroPropertyModule
    : public NFIHeroPropertyModule
{
public:
	NFCHeroPropertyModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCHeroPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

	virtual int OnHeroPropertyUpdate(const NFGUID& self, const NFGUID& xHeroGUID);

	virtual bool CalHeroBaseProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList);
	virtual bool CalHeroTalentProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList);
	virtual bool CalHeroEquipProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList);

protected:
	virtual int OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
	virtual int OnNPCClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

	virtual int OnObjectHeroRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
	

    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFIKernelModule* m_pKernelModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;
	NFIEquipPropertyModule* m_pEquipPropertyModule;
private:
};

#endif
