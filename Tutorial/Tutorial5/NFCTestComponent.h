// -------------------------------------------------------------------------
//    @FileName      :    NFCTestComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   NFCTestComponent
//
// -------------------------------------------------------------------------

#ifndef NFC_TEST_COMPONENT_H
#define NFC_TEST_COMPONENT_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPlugin.h"

class NFCTestComponent : public NFIComponent
{
public:
    NFCTestComponent(NFIPluginManager* p) : NFIComponent(NFGUID(), "")
    {
		pPluginManager = p;
    }
    virtual ~NFCTestComponent() {}


    virtual bool Init();
    virtual bool AfterInit();

	virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance();
    virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg);

private:
};

#endif
