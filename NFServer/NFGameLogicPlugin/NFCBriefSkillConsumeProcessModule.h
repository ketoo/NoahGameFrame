// -------------------------------------------------------------------------
//    @FileName      :   NFCBriefSkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBriefSkillConsumeProcessModule
//    @Desc             :   �������ѻ�����,�����࣬�����۳�������Ч
// -------------------------------------------------------------------------

#ifndef _NFC_BRIEFSKILLCONSUME_PROCESS_MODULE_H_
#define _NFC_BRIEFSKILLCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFComm/NFPluginModule/NFISkillConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISkillModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"

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
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    //��Ʒʹ���Ƿ�Ϸ�
    virtual int ConsumeLegal( const NFIDENTID& self, const std::string& skillID,  const NFIValueList& other );

    //ʹ����Ʒ������
    virtual int ConsumeSelf( const NFIDENTID& self, const std::string& skillID );

    //�Ϸ�,����,��ô�������[���ĺ�,nItemRowID�Ѿ��Ҳ����ˣ���Ϊ���ܱ������]
    virtual int ConsumeProcess( const NFIDENTID& self, const std::string& strSkillName, const NFIValueList& other, NFIValueList& damageListValue, NFIValueList& damageResultList );
    virtual int ConsumeProcessEx( const NFIDENTID& self, const std::string& strSkillName, const NFIValueList& other, NFIValueList& damageListValue, NFIValueList& damageResultList );

private:
    char* mstrSkillTableName;

    NFIKernelModule* m_pKernelModule;
    NFISkillModule* m_pSkillModule;
    NFIPropertyModule* m_pPropertyModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
