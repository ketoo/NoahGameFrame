#include "HelloWorld5Module.h"
#include "NFComm/NFCore/NFTimer.h"

bool HelloWorld5Module::Init()
{
    //��ʼ��
    std::cout << "Hello, world5, Init" << std::endl;

    return true;
}

bool HelloWorld5Module::AfterInit()
{
    //��ʼ�����
    std::cout << "Hello, world5, AfterInit" << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pPluginManager->FindModule("NFCLuaScriptModule"));

    assert( NULL != m_pLuaScriptModule);

    m_pKernelModule->CreateContainer(1, "");
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(NFIDENTID(), 1, 0, "Player", "", NFCDataList());
    if ( !pObject.get() )
    {
        return false;
    }

   

    return true;
}

bool HelloWorld5Module::Execute( const float fLasFrametime, const float fStartedTime )
{
    //ÿִ֡��
    //std::cout << "Hello, world5, Execute" << std::endl;

    return true;
}

bool HelloWorld5Module::BeforeShut()
{
    //����ʼ��֮ǰ
    std::cout << "Hello, world5, BeforeShut" << std::endl;

    m_pKernelModule->DestroyAll();

    return true;
}

bool HelloWorld5Module::Shut()
{
    //����ʼ��
    std::cout << "Hello, world5, Shut" << std::endl;

    return true;
}
