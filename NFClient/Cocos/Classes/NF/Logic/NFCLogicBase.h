// -------------------------------------------------------------------------
//    @FileName			:    NFCLogicBase.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCLogicBase
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGICBASE_H
#define NFC_LOGICBASE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFCore/NFSingleton.hpp"

class NFCLogicBase
    : public NFIModule
{
protected:	
	typedef std::function<int(const int, const NFDataList&)> MODULE_EVENT_FUNCTOR;
	typedef NF_SHARE_PTR<MODULE_EVENT_FUNCTOR> MODULE_EVENT_FUNCTOR_PTR;//EVENT

public:
	NFCLogicBase() {};
	virtual ~NFCLogicBase() {};
    NFCLogicBase(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();
	
	template<typename BaseType>
	bool AddEventCallBack(const int nEventID, BaseType* pBase, int (BaseType::*handler)(const int, const NFDataList&))
	{
		MODULE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		MODULE_EVENT_FUNCTOR_PTR functorPtr(new MODULE_EVENT_FUNCTOR(functor));
		mModuleEventPrtMap[functorPtr.get()] = pBase;
		return AddEventCallBack(nEventID, functorPtr);
	}
	virtual bool DoEvent(const int nEventID, const NFDataList& valueList);
	virtual bool RemoveEventCallBack(const int nEventID, void *pTarget);

protected:
	virtual bool AddEventCallBack(const int nEventID, const MODULE_EVENT_FUNCTOR_PTR cb);

	// 发送消息
public:

	// 接收消息
private:

public:

private:
	NFList<int> mModuleRemoveListEx;
	NFMapEx<int, NFList<MODULE_EVENT_FUNCTOR_PTR>> mModuleEventInfoMapEx;
	std::map<void*, void*> mModuleEventPrtMap;
};

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"

//class NFIClassModule;
//class NFIElementModule;
//class NFILogModule;
//class NFISceneAOIModule;
//class NFIKernelModule;
//class NFIEventModule;
//class NFIScheduleModule;
//class NFINetClientModule;

extern NFIClassModule *g_pClassModule;
extern NFIElementModule *g_pElementModule;
extern NFILogModule *g_pLogModule;
extern NFISceneAOIModule *g_pSceneModule;
extern NFIKernelModule *g_pKernelModule;
extern NFIEventModule *g_pEventModule;
extern NFIScheduleModule *g_pScheduleModule;
extern NFINetClientModule *g_pNetClientModule;

#endif