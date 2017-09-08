//
// Created by LUSHENG HUANG on 23/08/17.
//

#ifndef NOAHFRAME_NFCSKILLCOOLDOWNMODULE_H
#define NOAHFRAME_NFCSKILLCOOLDOWNMODULE_H


#include <NFComm/NFPluginModule/NFISkillModule.h>
#include <NFComm/NFCore/NFIObject.h>
#include <NFComm/NFPluginModule/NFINetModule.h>
#include <NFComm/NFPluginModule/NFIKernelModule.h>
#include <NFComm/NFPluginModule/NFIPropertyModule.h>
#include <NFComm/NFPluginModule/NFILogModule.h>
#include <NFComm/NFPluginModule/NFIEventModule.h>
#include <NFComm/NFPluginModule/NFISkillConsumeManagerModule.h>
#include <NFComm/NFPluginModule/NFIElementModule.h>
#include <NFComm/NFPluginModule/NFISceneProcessModule.h>
#include <NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h>
#include <NFComm/NFPluginModule/NFISkillCooldownModule.h>

class NFCSkillCooldownModule
        : public NFISkillCooldownModule
{
public:

    NFCSkillCooldownModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual bool AfterInit();

    virtual void AddSkillCD( const NFGUID& self, const std::string& strSkillName );
    virtual bool ExistSkillCD( const NFGUID& self, const std::string& strSkillName );

private:

    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
};

#endif //NOAHFRAME_NFCSKILLCOOLDOWNMODULE_H
