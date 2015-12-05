#include "Tutorial4ActorModule.h"
#include "NFComm/NFCore/NFTimer.h"
#include <thread>

bool HelloWorld4ActorModule::Init()
{
    //��ʼ��
    std::cout << "Hello, world4, Init ThreadID: " << std::this_thread::get_id() << std::endl;

    return true;
}

int HelloWorld4ActorModule::OnASyncEvent(const NFGUID& self, const int event, std::string& arg)
{
    //�¼��ص�����
    std::cout << "Begin OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << std::this_thread::get_id() << std::endl;

	arg += "  event test ok";

    return 0;
}

int HelloWorld4ActorModule::OnSyncEvent(const NFGUID& self, const int nActorID, const int event, const std::string& arg)
{
	//�¼��ص�����
	std::cout << "End OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << std::this_thread::get_id() << std::endl;

	return 0;
}

bool HelloWorld4ActorModule::AfterInit()
{

    //��ʼ�����
    std::cout << "Hello, world4, AfterInit, ThreadID: " << std::this_thread::get_id() << std::endl;

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

	//////////////////////////////////////ͬ��/////////////////////////////////////////////////////////////////////

	std::cout << "End Test Actor, ThreadID: " << std::this_thread::get_id() << std::endl;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////

    return true;
}

bool HelloWorld4ActorModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //ÿִ֡��
    //std::cout << "Hello, world3, Execute" << std::endl;

    return true;
}

bool HelloWorld4ActorModule::BeforeShut()
{
    //����ʼ��֮ǰ
    std::cout << "Hello, world4, BeforeShut" << std::endl;

    m_pKernelModule->DestroyAll();

    return true;
}

bool HelloWorld4ActorModule::Shut()
{
    //����ʼ��
    std::cout << "Hello, world4, Shut" << std::endl;

    return true;
}
