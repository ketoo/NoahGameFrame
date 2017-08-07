// -------------------------------------------------------------------------
//    @FileName      :   NFCBriefSkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBriefSkillConsumeProcessModule
// -------------------------------------------------------------------------

#ifndef NFC_BRIEFSKILLCONSUME_PROCESS_MODULE_H
#define NFC_BRIEFSKILLCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFISkillConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISkillModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCBriefSkillConsumeProcessModule
    : public NFISkillConsumeProcessModule
{
public:
    NFCBriefSkillConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual int ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFDataList& other );

    virtual int ConsumeProcess( const NFGUID& self, const std::string& strSkillName, const NFDataList& other, NFDataList& damageListValue, NFDataList& damageResultList );

private:
    NFIKernelModule* m_pKernelModule;
    NFISkillModule* m_pSkillModule;
    NFIPropertyModule* m_pPropertyModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementModule* m_pElementModule;
};

#endif
