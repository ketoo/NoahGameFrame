// -------------------------------------------------------------------------
//    @FileName      :    NFCEventRouteModule.h.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCEventRouteModule.h
//    @Desc			:	�¼�·�ɣ���Ҫ��������ֲ�ʽ����Щ�����ڱ�������Ȼ����Ҫת�����¼����ʼ���
// -------------------------------------------------------------------------

#ifndef _NFC_EVENTROUTE_MODULE_H_
#define _NFC_EVENTROUTE_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIEventRouteModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"

class NFCEventRouteModule
    : public NFIEventRouteModule
{
public:
    NFCEventRouteModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCEventRouteModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

public:
	virtual bool StartRouteEvent(const NFIDENTID& self, const NFIDataList& xDataList);

	virtual bool ReceiveRouteEvent(const NFIDENTID& self, const NFIDataList& xDataList);

private:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;

};


#endif
