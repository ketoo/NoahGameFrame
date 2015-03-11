#include "Tutorial4ActorModule.h"
#include "NFComm/NFCore/NFTimer.h"

bool HelloWorld4ActorModule::Init()
{
    //初始化
    std::cout << "Hello, world4, Init" << std::endl;

    return true;
}

int HelloWorld4ActorModule::OnEvent(const NFIDENTID& self, const int event, const NFIDataList& arg)
{
    //事件回调函数
    std::cout << "OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0) << " " << " " << arg.String(1) << std::endl;

    return 0;
}


bool HelloWorld4ActorModule::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world4, AfterInit" << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

	m_pEventProcessModule->AddEventCallBack(NFIDENTID(), 2222, this, &HelloWorld4ActorModule::OnEvent);

    m_pEventProcessModule->DoEvent(NFIDENTID(), 2222, NFCDataList() << 100 << "200", true);

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
