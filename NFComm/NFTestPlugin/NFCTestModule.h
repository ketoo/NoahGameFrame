// -------------------------------------------------------------------------
//    @FileName			:    NFCTestModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-06-11
//    @Module           :    NFCTestModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TEST_MODULE_H
#define NFC_TEST_MODULE_H

#include "Dependencies/googletest-release-1.8.0/googletest/include/gtest/gtest.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFITestModule
	: public NFIModule
{
};

class NFCTestModule
    : public NFITestModule
{
public:
    NFCTestModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFCTestModule() {};

    virtual bool Awake();
    virtual bool Init();
    virtual bool AfterInit();
    virtual bool CheckConfig();
    virtual bool ReadyExecute();
    virtual bool Execute();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Finalize();
    virtual bool OnReloadPlugin();

protected:

private:
};


#endif
