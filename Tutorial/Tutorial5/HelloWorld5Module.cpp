#include <thread>
#include "HelloWorld5Module.h"
#include "NFCTestComponent.h"
#include "NFComm/NFCore/NFTimer.h"
#include "NFComm/NFCore/NFIComponent.h"
#include "NFComm/NFCore/NFCObject.h"


bool HelloWorld5Module::Init()
{
    //初始化
    std::cout << "Hello, world5, Init" << std::endl;

    return true;
}

class IComponentTest : public NFIComponent
{
public:
	IComponentTest(): NFIComponent(NFIDENTID())
	{

	}


	virtual void Test() = 0;

protected:
private:
};

class ComponentTest : public IComponentTest
{
public:
	ComponentTest()
	{

	}

	virtual NFIComponent* CreateNewInstance()
	{
		return new ComponentTest();
	}

	virtual const std::string GetComponentName() const
	{
		return GET_CLASS_NAME(ComponentTest)
	}

	virtual void Test()
	{
		int i = 0; 
		++i;
	}

protected:
private:
};

bool HelloWorld5Module::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world5, AfterInit, ThreadID: " << std::this_thread::get_id() << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

	//////////////////////////////////////////////////////////////////////////

	NFIObject* pObject = new NFCObject(NFIDENTID(0, 1), pPluginManager);

	pObject->AddComponent<ComponentTest>();

	NF_SHARE_PTR<IComponentTest> pT = pObject->FindComponent<IComponentTest>("ComponentTest");
	pT->Test();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 50000; ++i)
	{
		int nActorID = pPluginManager->GetActorManager()->RequireActor<NFCTestComponent>(this, &HelloWorld5Module::OnSyncEvent);

		pPluginManager->GetActorManager()->SendMsgToActor(nActorID, NFIDENTID(), 555, boost::lexical_cast<std::string>(i));
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
