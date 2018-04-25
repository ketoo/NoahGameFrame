// -------------------------------------------------------------------------
//    @FileName			:		HelloWorld4.h
//    @Author           :		LvSheng.Huang
//    @Date             :		2017-02-06
//    @Module           :		HelloWorld4
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD4_H
#define NFC_HELLO_WORLD4_H

#include <thread>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFPluginModule/NFIComponent.h"

class NFCHttpComponent : public NFIComponent
{
public:
	NFCHttpComponent() : NFIComponent(GET_CLASS_NAME(NFCHttpComponent))
	{
	}

	virtual ~NFCHttpComponent()
	{

	}

	virtual bool Init()
	{
		AddMsgObserver(2, this, &NFCHttpComponent::OnMsgEvent);

		return true;
	}


	virtual int OnMsgEvent(const NFGUID& self, const int from, const int event, std::string& arg)
	{

		std::cout << "Thread: " << std::this_thread::get_id() << " " << self.ToString() << " MsgID: " << event << " Data:" << arg << std::endl;



		return 0;
	}

	virtual int OnASyncEvent(const NFGUID& self, const int from, const int event, std::string& arg)
	{
		std::cout << "------Actor thread: " << std::this_thread::get_id() << " " << self.ToString() << " MsgID: " << event << " Data:" << arg << std::endl;

		return 0;
	}
};

class NFIHelloWorld4Module
	: public NFIModule
{
};

class NFCHelloWorld4Module
    : public NFIHelloWorld4Module
{
public:
    NFCHelloWorld4Module(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	int HttpRequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData);
	
protected:
    NFIActorModule* m_pActorModule;
};

#endif
