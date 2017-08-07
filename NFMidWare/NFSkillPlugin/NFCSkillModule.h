// -------------------------------------------------------------------------
//    @FileName      :    NFCSkillModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-06-11
//    @Module           :    NFCSkillModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SKILL_MODULE_H
#define NFC_SKILL_MODULE_H

#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISkillModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
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

    virtual int ExistSkill( const NFGUID& self, const std::string& strSkillName );

protected:
    int OnClassObjectEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var );


protected:
	void OnClienUseSkill(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:

	NFINetModule* m_pNetModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIEventModule* m_pEventModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
