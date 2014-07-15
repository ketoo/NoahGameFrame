// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCPotionConsumeProcessModule
//    @Desc             :   �������ѻ�����--ʹ����Ʒ����ü��ܻ��ߵ���
//                                  --������ѧϰ����,���������Ʒ,BUFF��õ�BUFF,������ʹ�ü���
// -------------------------------------------------------------------------

#ifndef _NFC_CARDITEMCONSUME_PROCESS_MODULE_H_
#define _NFC_CARDITEMCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCCardItemConsumeProcessModule
    : public NFIItemConsumeProcessModule
{

public:
    NFCCardItemConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    //��Ʒʹ���Ƿ�Ϸ�
    virtual int ConsumeLegal( const NFIDENTID& self, int nItemRowID,  const NFIValueList& other );

    //ʹ����Ʒ������
    virtual int ConsumeSelf( const NFIDENTID& self, int nItemRowID );

    //�Ϸ�,����,��ô�������[���ĺ�,nItemRowID�Ѿ��Ҳ����ˣ���Ϊ���ܱ������]
    virtual int ConsumeProcess( const NFIDENTID& self, const std::string& strItemName, const NFIValueList& other );

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
    NFIPackModule* m_pPackModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
