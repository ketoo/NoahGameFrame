#include "HelloWorld5Module.h"
#include "NFComm/NFCore/NFTimer.h"
#include <thread>
#include "NFComm/NFCore/NFIComponent.h"
#include "NFCTestComponent.h"



bool HelloWorld5Module::Init()
{
    //初始化
    std::cout << "Hello, world5, Init" << std::endl;

    return true;
}

bool HelloWorld5Module::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world5, AfterInit, ThreadID: " << std::this_thread::get_id() << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 50000; ++i)
	{
		int nActorID = m_pEventProcessModule->AddActorEventCallBack<NFCTestComponent>(this, &HelloWorld5Module::OnSyncEvent);
		m_pEventProcessModule->SendActorMsg(nActorID, 555, NFIDENTID(), boost::lexical_cast<std::string>(i));
	}

    std::cout << "End Test Actor, ThreadID: " << std::this_thread::get_id() << std::endl;

    return true;
}

int HelloWorld5Module::OnSyncEvent(const NFIDENTID& self, const int nActorID, const int event, const std::string& arg)
{
    //事件回调函数
    std::cout << "End OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << std::this_thread::get_id() << std::endl;

    return 0;
}

bool HelloWorld5Module::Execute( const float fLasFrametime, const float fStartedTime )
{
    //每帧执行
    //std::cout << "Hello, world5, Execute" << std::endl;

    return true;
}

bool HelloWorld5Module::BeforeShut()
{
    //反初始化之前
    std::cout << "Hello, world5, BeforeShut" << std::endl;

    m_pKernelModule->DestroyAll();

    return true;
}

bool HelloWorld5Module::Shut()
{
    //反初始化
    std::cout << "Hello, world5, Shut" << std::endl;

    return true;
}
