// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD_H
#define NFC_HELLO_WORLD_H

#include <iostream>

#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"



class NFIHelloWorld
	: public NFIModule
{

};

class NFCHelloWorld
    : public NFIHelloWorld
{
public:
    NFCHelloWorld(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	bool OnCommandQuery(const NFHttpRequest& req);

	bool OnCommonQuery(const NFHttpRequest& req);

	NFWebStatus OnFilter(const NFHttpRequest& req);

	int OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

	void OnGetCallBack(const NFGUID id, const int state_code, const std::string& strRespData);
	void OnPostCallBack(const NFGUID id, const int state_code, const std::string& strRespData);
private:

	NFIClassModule* m_pLogicClassModule;
	NFIHttpServerModule* m_pHttpNetModule;
	NFIHttpClientModule* m_pHttpClientModule;
	NFIElementModule* m_pElementModule;
	NFIScheduleModule* m_pScheduleModule;
};

#endif
