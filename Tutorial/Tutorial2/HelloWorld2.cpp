#include "HelloWorld2.h"
#include "NFComm\NFCore\NFCObject.h"

bool HelloWorld2::Init()
{
    //��ʼ��

    std::cout << "Hello, world2, Init" << std::endl;

    return true;
}

int HelloWorld2::OnPropertyCallBackEvent( const NFIDENTID& self, const std::string& strProperty, const NFIDataList& oldVarList, const NFIDataList& newVarList, const NFIDataList& argVarList )
{
    //���Իص��¼���ֻҪ����ֵ�����б仯���ͻᱻ�ص�
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVarList.Int(0) << " NewValue: " << newVarList.Int(0) << std::endl;

    return 0;
}

bool HelloWorld2::AfterInit()
{
    if(pPluginManager->GetActorID() == NFIActorManager::EACTOR_MAIN)
    {
        //��ʼ�����
        std::cout << "Hello, world2, AfterInit" << std::endl;

        NFIObject* pObject = new NFCObject(1, pPluginManager);
        pObject->GetPropertyManager()->AddProperty(pObject->Self(), "Hello", TDATA_STRING, true, true, true, true, 0, "");
        pObject->GetPropertyManager()->AddProperty(pObject->Self(), "World", TDATA_INT, true, true, true, true, 0, "");

        pObject->SetPropertyInt("World", 1111);

        const int nProperty1 = pObject->GetPropertyInt("World");
        std::cout << "Property World:" << nProperty1 << std::endl;

        //��������¼�
        pObject->AddPropertyCallBack("World", this, &HelloWorld2::OnPropertyCallBackEvent);

        pObject->SetPropertyInt("World", 2222);

        const int nProperty2 = pObject->GetPropertyInt("World");
        std::cout << "Property World:" << nProperty2 << std::endl;
    }
    

    return true;
}

bool HelloWorld2::Execute( const float fLasFrametime, const float fStartedTime )
{
    //ÿִ֡��
    //std::cout << "Hello, world2, Execute" << std::endl;

    return true;
}

bool HelloWorld2::BeforeShut()
{
    //����ʼ��֮ǰ
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool HelloWorld2::Shut()
{
    //����ʼ��
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}
