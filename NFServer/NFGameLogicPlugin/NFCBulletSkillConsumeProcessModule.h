// -------------------------------------------------------------------------
//    @FileName      :   NFCBulletSkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBulletSkillConsumeProcessModule
//    @Desc             :   �������ѻ�����,�ӵ��࣬�����۳������ǵ������Ч
// -------------------------------------------------------------------------

#ifndef NFC_BULLETSKILLCONSUME_PROCESS_MODULE_H
#define NFC_BULLETSKILLCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCBulletSkillConsumeProcessModule
    : public NFISkillConsumeProcessModule
{
public:
    NFCBulletSkillConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    //��Ʒʹ���Ƿ�Ϸ�
    virtual int ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFIDataList& other );

    //ʹ����Ʒ������
    virtual int ConsumeSelf( const NFGUID& self, const std::string& skillID );

    virtual int ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList );
    virtual int ConsumeProcessEx( const NFGUID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList );

private:
    NFIKernelModule* m_pKernelModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementInfoModule* m_pElementInfoModule;


};

#endif
