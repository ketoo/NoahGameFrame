#include "HelloWorld3Module.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool HelloWorld3Module::Init()
{
	//初始化
	std::cout << "Hello, world3, Init" << std::endl;

	return true;
}

int HelloWorld3Module::OnEvent(const NFGUID& self, const NFEventDefine event, const NFIDataList& arg)
{
	//事件回调函数
	std::cout << "OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0) << " " << " " << arg.String(1) << std::endl;

	m_pKernelModule->SetPropertyInt(self, "Hello", arg.Int(0));
	m_pKernelModule->SetPropertyString(self, "Hello", arg.String(1));

	return 0;
}

int HelloWorld3Module::OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{

	unsigned long unNowTime = m_pKernelModule->GetTime();

	std::cout << "strHeartBeat: " << fTime << " Count: " << nCount << "  TimeDis: " << unNowTime - mLastTime << std::endl;

	mLastTime = unNowTime;

	return 0;
}

int HelloWorld3Module::OnClassCallBackEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFIDataList& arg)
{
	//虚拟类事件，只要有此虚拟类创建或者销毁即会回调
	std::cout << "OnClassCallBackEvent ClassName: " << strClassName << " ID: " << self.nData64 << " Event: " << event << std::endl;

	if (event == COE_CREATE_HASDATA)
	{
		m_pEventModule->AddEventCallBack(self, NFEventDefine(1), this, &HelloWorld3Module::OnEvent);

		m_pScheduleModule->AddSchedule(self, "OnHeartBeat", this, &HelloWorld3Module::OnHeartBeat, 5.0f, 10 );

		mLastTime = m_pKernelModule->GetTime();
	}

	return 0;
}

int HelloWorld3Module::OnPropertyCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	//属性回调事件，只要属性值内容有变化，就会被回调
	std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetInt() << " NewValue: " << newVar.GetInt() << std::endl;

	return 0;
}

int HelloWorld3Module::OnPropertyStrCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	//属性回调事件，只要属性值内容有变化，就会被回调
	std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetString() << " NewValue: " << newVar.GetString() << std::endl;

	return 0;
}

bool HelloWorld3Module::AfterInit()
{
	//初始化完毕
	std::cout << "Hello, world3, AfterInit" << std::endl;

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	
	//创建容器，所有的对象均需在容器中
	m_pKernelModule->CreateScene(1);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &HelloWorld3Module::OnClassCallBackEvent);

	//创建对象，挂类回调和属性回调,然后事件处理对象
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(NFGUID(0, 10), 1, 0, NFrame::Player::ThisName(), "", NFCDataList());
	if (!pObject.get())
	{
		return false;
	}

	pObject->GetPropertyManager()->AddProperty(pObject->Self(), "Hello", TDATA_STRING);
	pObject->GetPropertyManager()->AddProperty(pObject->Self(), "World", TDATA_INT);

	pObject->AddPropertyCallBack("Hello", this, &HelloWorld3Module::OnPropertyStrCallBackEvent);
	pObject->AddPropertyCallBack("World", this, &HelloWorld3Module::OnPropertyCallBackEvent);

	pObject->SetPropertyString("Hello", "hello,World");
	pObject->SetPropertyInt("World", 1111);


	m_pEventModule->DoEvent(pObject->Self(), NFEventDefine(1), NFCDataList() << int(100) << "200");

	return true;
}

bool HelloWorld3Module::Execute()
{
	//每帧执行
	//std::cout << "Hello, world3, Execute" << std::endl;

	return true;
}

bool HelloWorld3Module::BeforeShut()
{
	//反初始化之前
	std::cout << "Hello, world3, BeforeShut" << std::endl;

	m_pKernelModule->DestroyAll();

	return true;
}

bool HelloWorld3Module::Shut()
{
	//反初始化
	std::cout << "Hello, world3, Shut" << std::endl;

	return true;
}
