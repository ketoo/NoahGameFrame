// -------------------------------------------------------------------------
//    @FileName			:		HelloWorld4.h
//    @Author           :		LvSheng.Huang
//    @Date             :		2017-02-06
//    @Module           :		HelloWorld4
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD4_H
#define NFC_HELLO_WORLD4_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCHttpComponent : public NFIComponent
{
public:
	NFCHttpComponent(NFIPluginManager* pPluginManager) : NFIComponent(NFGUID(), "")
	{
	}

	NFCHttpComponent(NFGUID self, const std::string& strName) : NFIComponent(self, strName)
	{
	}

	virtual ~NFCHttpComponent()
	{

	}

	virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg)
	{
		std::cout << "Hello, welcome to actor thread: " << self.ToString() << " MsgID: " << event << " Data:" << arg << std::endl;

		return 0;
	}

protected:
	virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance()
	{
		return  NF_SHARE_PTR<NFIComponent>(NF_NEW  NFCHttpComponent(NFGUID(1, 2), ""));
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
