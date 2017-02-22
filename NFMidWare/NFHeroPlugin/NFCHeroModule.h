// -------------------------------------------------------------------------
//    @FileName			:    NFCHeroModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-16
//    @Module           :    NFCHeroModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HERO_MODULE_MODULE_H
#define NFC_HERO_MODULE_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIGuildEctypeModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIHeroPropertyModule.h"

class NFCHeroModule
    : public NFIHeroModule
{
public:
    NFCHeroModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCHeroModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual NFGUID AddHero(const NFGUID& self, const std::string& strID);
    virtual bool AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int nExp);
    virtual bool HeroStarUp(const NFGUID& self, const NFGUID& xHeroID);
    virtual bool HeroSkillUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex);
    virtual bool HeroTalentUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex);

	virtual bool HeroWearSkill(const NFGUID& self, const NFGUID& xHeroID, const std::string& xSkillID);
	virtual bool SetFightHero(const NFGUID& self, const int nPos, const NFGUID& xHeroID);

	virtual bool CreateHero(const NFGUID& self, const NFGUID& xHeroID);
	virtual bool DestroyHero(const NFGUID& self, const NFGUID& xHeroID);
	virtual NFGUID GetHeroGUID(const NFGUID& self, const std::string& strID);

protected:
	void OnSetFightHeroMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

protected:
    NFIClassModule* m_pLogicClassModule;
    NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
	NFINetModule* m_pNetModule;
	NFIHeroPropertyModule* m_pHeroPropertyModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFISceneProcessModule* m_pSceneProcessModule;
private:
};

#endif
