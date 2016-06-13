#include <thread>
#include "HelloWorld5Module.h"
#include "NFCTestComponent.h"
#include "NFComm/NFCore/NFTimer.h"
#include "NFComm/NFCore/NFIComponent.h"
#include "NFComm/NFCore/NFCObject.h"


bool HelloWorld5Module::Init()
{
    //初始化
    std::cout << "Hello, world5, Init, ThreadID: " << std::this_thread::get_id() << std::endl;

    return true;
}

class IComponentTest : public NFIComponent
{
public:
	IComponentTest(): NFIComponent(NFGUID(), "ComponentTest")
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

	virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance()
	{
		return NF_SHARE_PTR<NFIComponent>(NF_NEW ComponentTest());
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

    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	////////////////////////test1//////////////////////////////////////////////////

	NFIObject* pObject = new NFCObject(NFGUID(0, 1), pPluginManager);

	pObject->AddComponent<ComponentTest>();

	NF_SHARE_PTR<IComponentTest> pT = pObject->FindComponent<IComponentTest>("ComponentTest");
	pT->Test();

	///////////////////////////test2////////////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIComponent> pNewAddCOmponent = pObject->AddComponent<NFIComponent>("NFCTestComponent");

	////////////////////////////test3///////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 1; ++i)
	{
		int nActorID = pPluginManager->GetActorManager()->RequireActor<NFCTestComponent>(this, &HelloWorld5Module::OnSyncEvent);

		pPluginManager->GetActorManager()->SendMsgToActor(nActorID, NFGUID(), 555, lexical_cast<std::string>(i));
	}

    std::cout << "End Test Actor, ThreadID: " << std::this_thread::get_id() << std::endl;

    return true;
}

int HelloWorld5Module::OnSyncEvent(const NFGUID& self, const int nActorID, const int event, const std::string& arg)
{
    //事件回调函数
    std::cout << "End OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << std::this_thread::get_id() << std::endl;

    return 0;
}

bool HelloWorld5Module::Execute()
{
    //每帧执行
    //std::cout << "Hello, world5, Execute" << std::endl;

    return true;
}

bool HelloWorld5Module::BeforeShut()
{
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
