#include "HelloWorld3Module.h"
#include "NFComm/NFCore/NFTimer.h"

bool HelloWorld3Module::Init()
{
    //初始化
    std::cout << "Hello, world3, Init" << std::endl;

    return true;
}

int HelloWorld3Module::OnEvent(const NFIDENTID& self, const int event, const NFIDataList& arg)
{
    //事件回调函数
    std::cout << "OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0) << " " << " " << arg.String(1) << std::endl;

    m_pKernelModule->SetPropertyInt(self, "Hello", arg.Int(0));
    m_pKernelModule->SetPropertyString(self, "Hello", arg.String(1));

    return 0;
}

int HelloWorld3Module::OnHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& arg)
{

    unsigned long unNowTime = NF_GetTickCount();

    std::cout << "strHeartBeat: " << fTime << " Count: " << nCount << "  TimeDis: " << unNowTime - mLastTime << std::endl;

    mLastTime = unNowTime;

    return 0;
}

int HelloWorld3Module::OnClassCallBackEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFIDataList& arg)
{
    //虚拟类事件，只要有此虚拟类创建或者销毁即会回调
    std::cout << "OnClassCallBackEvent ClassName: " << strClassName << " ID: " << self.nData64 << " Event: " << event << std::endl;

    if (event == COE_CREATE_HASDATA)
    {
#ifdef NF_USE_ACTOR
        if(pPluginManager->GetActorID() == NFIActorManager::EACTOR_MAIN)
#endif
        {
            m_pEventProcessModule->AddEventCallBack(self, 11111111, this, &HelloWorld3Module::OnEvent);

            m_pKernelModule->AddHeartBeat(self, "OnHeartBeat", this, &HelloWorld3Module::OnHeartBeat, NFCDataList(), 5.0f, 9999 );

            mLastTime = NF_GetTickCount();
        }
    }

    return 0;
}

int HelloWorld3Module::OnPropertyCallBackEvent( const NFIDENTID& self, const std::string& strProperty, const NFIDataList& oldVarList, const NFIDataList& newVarList, const NFIDataList& argVarList )
{
    //属性回调事件，只要属性值内容有变化，就会被回调
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVarList.Int(0) << " NewValue: " << newVarList.Int(0) << std::endl;

    return 0;
}

int HelloWorld3Module::OnPropertyStrCallBackEvent( const NFIDENTID& self, const std::string& strProperty, const NFIDataList& oldVarList, const NFIDataList& newVarList, const NFIDataList& argVarList )
{
    //属性回调事件，只要属性值内容有变化，就会被回调
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVarList.String(0) << " NewValue: " << newVarList.String(0) << std::endl;

    return 0;
}

bool HelloWorld3Module::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world3, AfterInit" << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    //创建容器，所有的对象均需在容器中
    m_pKernelModule->CreateContainer(1, "");

    m_pEventProcessModule->AddClassCallBack("Player", this, &HelloWorld3Module::OnClassCallBackEvent);

    //创建对象，挂类回调和属性回调,然后事件处理对象
    std::shared_ptr<NFIObject> pObject = m_pKernelModule->CreateObject(NFIDENTID(0, 10), 1, 0, "Player", "", NFCDataList());
    if (!pObject.get())
    {
        return false;
    }

    pObject->GetPropertyManager()->AddProperty(pObject->Self(), "Hello", TDATA_STRING, true, true, true, true, 0, "");
    pObject->GetPropertyManager()->AddProperty(pObject->Self(), "World", TDATA_INT, true, true, true, true, 0, "");

    pObject->AddPropertyCallBack("Hello", this, &HelloWorld3Module::OnPropertyStrCallBackEvent);
    pObject->AddPropertyCallBack("World", this, &HelloWorld3Module::OnPropertyCallBackEvent);

    pObject->SetPropertyString("Hello", "hello,World");
    pObject->SetPropertyInt("World", 1111);


    m_pEventProcessModule->DoEvent(pObject->Self(), 11111111, NFCDataList() << int(100) << "200");

    return true;
}

bool HelloWorld3Module::Execute( const float fLasFrametime, const float fStartedTime )
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
