// -------------------------------------------------------------------------
//    @FileName      :   NFCItemConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCItemConsumeManagerModule
//    @Desc             :   �������ѻ��ƹ�����,�������͵ĵ����������;���ע���������
// -------------------------------------------------------------------------

#ifndef _NFC_ITEMCONSUME_MANAGER_MODULE_H_
#define _NFC_ITEMCONSUME_MANAGER_MODULE_H_

#include <iostream>
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"

class NFCItemConsumeManagerModule
    : public NFIItemConsumeManagerModule,
      NFMap<int, NFIItemConsumeProcessModule>
{
public:

    NFCItemConsumeManagerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual bool ResgisterConsumeModule( const int nModuleType, NFIItemConsumeProcessModule* pModule );

    virtual NFIItemConsumeProcessModule* GetConsumeModule( const int nModuleType );

};

#endif
