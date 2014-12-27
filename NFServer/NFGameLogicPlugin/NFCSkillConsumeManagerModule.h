// -------------------------------------------------------------------------
//    @FileName      :   NFCSkillConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBuSkillConsumeManagerModule
//    @Desc             :   �������ѻ��ƹ�����,�������͵ļ����������;���ע���������
// -------------------------------------------------------------------------

#ifndef _NFC_SKILLCONSUME_MANAGER_MODULE_H_
#define _NFC_SKILLCONSUME_MANAGER_MODULE_H_

#include <iostream>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeProcessModule.h"

class NFCSkillConsumeManagerModule
    : public NFISkillConsumeManagerModule,
      NFMap<int, NFISkillConsumeProcessModule>
{
public:
    NFCSkillConsumeManagerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual bool ResgisterConsumeModule( const int nModuleType, NFISkillConsumeProcessModule* pModule );

    virtual NFISkillConsumeProcessModule* GetConsumeModule( const int nModuleType );

};

#endif
