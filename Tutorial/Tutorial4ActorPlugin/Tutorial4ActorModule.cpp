#include "Tutorial4ActorModule.h"
#include "NFComm/NFCore/NFTimer.h"

bool HelloWorld4ActorModule::Init()
{
    //初始化
    std::cout << "Hello, world4, Init ThreadID: " << GetCurrentThreadId() << std::endl;

    return true;
}

int HelloWorld4ActorModule::OnASyncEvent(const NFIDENTID& self, const int event, std::string& arg)
{
    //事件回调函数
    std::cout << "Begin OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << GetCurrentThreadId() << std::endl;

	arg += "  event test ok";

    return 0;
}

int HelloWorld4ActorModule::OnSyncEvent(const NFIDENTID& self, const int event, const std::string& arg)
{
	//事件回调函数
	std::cout << "End OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << GetCurrentThreadId() << std::endl;

	return 0;
}

bool HelloWorld4ActorModule::AfterInit()
{

    //初始化完毕
    std::cout << "Hello, world4, AfterInit, ThreadID: " << GetCurrentThreadId() << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

	//////////////////////////////////////同步/////////////////////////////////////////////////////////////////////
	m_pEventProcessModule->AddAsyncEventCallBack(NFIDENTID(), 2222, this, &HelloWorld4ActorModule::OnASyncEvent, &HelloWorld4ActorModule::OnSyncEvent);

	for (int i = 0; i < 20; ++i)
	{
		m_pEventProcessModule->DoEvent(NFIDENTID(), 2222, NFCDataList() << boost::lexical_cast<std::string>(i), false);

	}

	std::cout << "End Test Actor, ThreadID: " << GetCurrentThreadId() << std::endl;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////

    return true;
}

bool HelloWorld4ActorModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //每帧执行
    //std::cout << "Hello, world3, Execute" << std::endl;

    return true;
}

bool HelloWorld4ActorModule::BeforeShut()
{
    //反初始化之前
    std::cout << "Hello, world4, BeforeShut" << std::endl;

    m_pKernelModule->DestroyAll();

    return true;
}

bool HelloWorld4ActorModule::Shut()
{
    //反初始化
    std::cout << "Hello, world4, Shut" << std::endl;

    return true;
}
