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
#include "NFComm/NFPluginModule/NFIGuildEctypeModule.h"

class NFCHeroPropertyModule
    : public NFIHeroPropertyModule
{
public:
	NFCHeroPropertyModule( NFIPluginManager* p )
    {
		std::string s = NFrame::BB_Build::NormalStateFunc();
        pPluginManager = p;
    }
    virtual ~NFCHeroPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

	virtual int OnHeroPropertyUpdate(const NFGUID& self, const NFGUID& xHeroID);
protected:
	virtual int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

	virtual int OnObjectHeroRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

	int CalHeroPropertySelf(const NFGUID & self, const NFGUID & xHeroID, NFIDataList & xPpropertyManager);

	int CalHeroPropertyTalent(const NFGUID & self, const NFGUID & xHeroID, NFIDataList & xPpropertyManager);

	int CalHeroPropertyEquip(const NFGUID & self, const NFGUID & xHeroID, NFIDataList & xPpropertyManager);

	

    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFIKernelModule* m_pKernelModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIGuildEctypeModule* m_pGuildEctypeModule;
private:
};

#endif
