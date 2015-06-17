#include "NFCTestComponent.h"
#include "NFComm/NFCore/NFTimer.h"

bool NFCTestComponent::Init()
{
    //��ʼ��
    std::cout << "Hello, NFCTestComponent, Init" << std::endl;

    return true;
}

bool NFCTestComponent::AfterInit()
{
    //��ʼ�����
    std::cout << "Hello, NFCTestComponent, AfterInit" << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

	assert( NULL != m_pKernelModule);
	assert( NULL != m_pEventProcessModule);
	assert( NULL != m_pElementInfoModule);

    m_pKernelModule->CreateContainer(1, "");
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(NFIDENTID(), 1, 0, "Player", "", NFCDataList());
    if ( !pObject.get() )
    {
        return false;
    }

   //pObject->AddComponent();

    return true;
}

bool NFCTestComponent::Execute( const float fLasFrametime, const float fStartedTime )
{
    //ÿִ֡��
    //std::cout << "Hello, NFCTestComponent, Execute" << std::endl;

    return true;
}

bool NFCTestComponent::BeforeShut()
{
    //����ʼ��֮ǰ
    std::cout << "Hello, NFCTestComponent, BeforeShut" << std::endl;

    m_pKernelModule->DestroyAll();

    return true;
}

bool NFCTestComponent::Shut()
{
    //����ʼ��
    std::cout << "Hello, NFCTestComponent, Shut" << std::endl;

    return true;
}
