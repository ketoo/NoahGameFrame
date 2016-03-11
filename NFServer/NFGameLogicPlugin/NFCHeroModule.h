// -------------------------------------------------------------------------
//    @FileName      :    NFCHeroModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCHeroModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HERO_MODULE_MODULE_H
#define NFC_HERO_MODULE_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCHeroModule
    : public NFIHeroModule
{
public:
    NFCHeroModule( NFIPluginManager* p )
    {
		std::string s = NFrame::BB_Build::NormalStateFunc();
        pPluginManager = p;
    }
    virtual ~NFCHeroModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual bool AddHero(const NFGUID& self, const std::string& strID);
    virtual bool AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int nExp);
    virtual bool HeroStarUp(const NFGUID& self, const NFGUID& xHeroID);
    virtual bool HeroSkillUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex);
    virtual bool HeroTalentUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex);
    virtual bool SetFightHero(const NFGUID& self, const NFGUID& xID);

protected:
	void OnSetFightHeroProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
    int OnObjectFightHeroEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

protected:


protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFIKernelModule* m_pKernelModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;
private:
};

#endif
