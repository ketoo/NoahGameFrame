#include "HelloWorld3Module.h"
#include "NFComm\NFCore\NFTimer.h"

bool HelloWorld3Module::Init()
{
    //��ʼ��
    std::cout << "Hello, world3, Init" << std::endl;

    return true;
}

int HelloWorld3Module::OnEvent(const NFIDENTID& self, const int event, const NFIValueList& arg)
{
    //�¼��ص�����
    std::cout << "OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.IntVal(0) << " " << " " << arg.StringVal(1) << std::endl;

    m_pKernelModule->SetPropertyInt(self, "Hello", arg.IntVal(0));
    m_pKernelModule->SetPropertyString(self, "Hello", arg.StringVal(1));

    return 0;
}

int HelloWorld3Module::OnHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& arg)
{
    unsigned long unNowTime = ::GetTickCount();

    std::cout << "strHeartBeat: " << fTime << " Count: " << nCount << "  TimeDis: " << unNowTime - mLastTime << std::endl;

    mLastTime = unNowTime;

    return 0;
}

int HelloWorld3Module::OnClassCallBackEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFIValueList& arg)
{
    //�������¼���ֻҪ�д������ഴ���������ټ���ص�
    std::cout << "OnClassCallBackEvent ClassName: " << strClassName << " ID: " << self.nData64 << " Event: " << event << std::endl;

    if (event == COE_CREATE_HASDATA)
    {

        if(pPluginManager->GetActor() == NFIActorManager::EACTOR_MAIN)
        {
            m_pEventProcessModule->AddEventCallBack(self, 11111111, this, &HelloWorld3Module::OnEvent);

            m_pKernelModule->AddHeartBeat(self, "OnHeartBeat", this, &HelloWorld3Module::OnHeartBeat, NFCValueList(), 5.0f, 9999 );

            mLastTime = ::GetTickCount();
        }
    }

    return 0;
}

int HelloWorld3Module::OnPropertyCallBackEvent( const NFIDENTID& self, const std::string& strProperty, const NFIValueList& oldVarList, const NFIValueList& newVarList, const NFIValueList& argVarList )
{
    //���Իص��¼���ֻҪ����ֵ�����б仯���ͻᱻ�ص�
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVarList.IntVal(0) << " NewValue: " << newVarList.IntVal(0) << std::endl;

    return 0;
}

int HelloWorld3Module::OnPropertyStrCallBackEvent( const NFIDENTID& self, const std::string& strProperty, const NFIValueList& oldVarList, const NFIValueList& newVarList, const NFIValueList& argVarList )
{
    //���Իص��¼���ֻҪ����ֵ�����б仯���ͻᱻ�ص�
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVarList.StringVal(0) << " NewValue: " << newVarList.StringVal(0) << std::endl;

    return 0;
}

bool HelloWorld3Module::AfterInit()
{
    //��ʼ�����
    std::cout << "Hello, world3, AfterInit" << std::endl;

    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    //�������������еĶ��������������
    m_pKernelModule->CreateContainer(1, "");

    m_pEventProcessModule->AddClassCallBack("Player", this, &HelloWorld3Module::OnClassCallBackEvent);

    //�������󣬹���ص������Իص�,Ȼ���¼��������
    NFIObject* pObject = m_pKernelModule->CreateObject(10, 1, 0, "Player", "", NFCValueList());
    
    pObject->GetPropertyManager()->AddProperty(pObject->Self(), "Hello", VTYPE_STRING, true, true, true, 0, "");
    pObject->GetPropertyManager()->AddProperty(pObject->Self(), "World", VTYPE_INT, true, true, true, 0, "");

    pObject->AddPropertyCallBack("Hello", this, &HelloWorld3Module::OnPropertyStrCallBackEvent);
    pObject->AddPropertyCallBack("World", this, &HelloWorld3Module::OnPropertyCallBackEvent);

    pObject->SetPropertyString("Hello", "hello,World");
    pObject->SetPropertyInt("World", 1111);


    m_pEventProcessModule->DoEvent(pObject->Self(), 11111111, NFCValueList() << int(100) << "200");

    return true;
}

bool HelloWorld3Module::Execute( const float fLasFrametime, const float fStartedTime )
{
    //ÿִ֡��
    //std::cout << "Hello, world3, Execute" << std::endl;

    return true;
}

bool HelloWorld3Module::BeforeShut()
{
    //����ʼ��֮ǰ
    std::cout << "Hello, world3, BeforeShut" << std::endl;

    m_pKernelModule->DestroyAll();

    return true;
}

bool HelloWorld3Module::Shut()
{
    //����ʼ��
    std::cout << "Hello, world3, Shut" << std::endl;

    return true;
}
