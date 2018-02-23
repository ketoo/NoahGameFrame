// -------------------------------------------------------------------------
//    @FileName			:		HelloWorld4.cpp
//    @Author           :		LvSheng.Huang
//    @Date             :		2017-02-06
//    @Module           :		HelloWorld4
//
// -------------------------------------------------------------------------

#include "HelloWorld4Module.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCHelloWorld4Module::Init()
{
	return true;
}

int NFCHelloWorld4Module::HttpRequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData)
{
	std::cout << "Main thread: " << std::this_thread::get_id() << " " << self.ToString() << " Actor: " << nFormActor << " MsgID: " << nSubMsgID << " Data:" << strData << std::endl;

	return 0;
}

bool NFCHelloWorld4Module::AfterInit()
{
	
	std::cout << "Hello, world4, AfterInit" << std::endl;

	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();

	int nActorID = m_pActorModule->RequireActor();
	m_pActorModule->AddComponent<NFCHttpComponent>(nActorID);
	m_pActorModule->AddDefaultEndFunc(nActorID, this, &NFCHelloWorld4Module::HttpRequestAsyEnd);

	for (int i = 0; i < 10; ++i)
	{
		m_pActorModule->SendMsgToActor(nActorID, NFGUID(10, 20), i, "Test actor!");
	}

	std::cout << "Hello, world4, AfterInit end" << std::endl;

	return true;
}

bool NFCHelloWorld4Module::Execute()
{
	
	//std::cout << "Hello, world4, Execute" << std::endl;

	return true;
}

bool NFCHelloWorld4Module::BeforeShut()
{
	
	std::cout << "Hello, world4, BeforeShut" << std::endl;

	return true;
}

bool NFCHelloWorld4Module::Shut()
{
	
	std::cout << "Hello, world4, Shut" << std::endl;

	return true;
}
