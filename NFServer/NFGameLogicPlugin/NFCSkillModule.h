// -------------------------------------------------------------------------
//    @FileName      :    NFCSkillModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-06-11
//    @Module           :    NFCSkillModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SKILL_MODULE_H
#define NFC_SKILL_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFISkillModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCSkillModule
    : public NFISkillModule
{
public:

    NFCSkillModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCSkillModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual int AddSkill( const NFGUID& self, const std::string& strSkillName );
    virtual int ExistSkill( const NFGUID& self, const std::string& strSkillName );
    virtual int SetSkillLevel( const NFGUID& self, const std::string& strSkillName, const int nLevel );
    virtual int SetSkillGem( const NFGUID& self, const std::string& strSkillName, const std::string& strGemName );
    virtual int SetSkillGemLevel( const NFGUID& self, const std::string& strSkillName, const int nLevel );

    virtual int GetSkillLevel( const NFGUID& self, const std::string& strSkillName );
    virtual std::string GetSkillGem( const NFGUID& self, const std::string& strSkillName );
    virtual int GetSkillGemLevel( const NFGUID& self, const std::string& strSkillName );

    virtual int AddNewerSkill( const NFGUID& self );
    virtual int OnUseSkill(const NFGUID& self, const NFIDataList& var);
protected:

    int OnClassObjectEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

	int OnRequireUseSkillEvent( const NFGUID& self, const int nEventID, const NFIDataList& var );
	int OnRequireUseSkillPosEvent( const NFGUID& self, const int nEventID, const NFIDataList& var );

protected:
	void OnClienUseSkill(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
private:
    char* mstrSkillTableName;

    NFIPropertyModule* m_pPropertyModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFISceneProcessModule* m_pSceneProcessModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
