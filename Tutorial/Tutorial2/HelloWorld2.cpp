#include "HelloWorld2.h"
#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFIComponent.h"

bool HelloWorld2::Init()
{
    //初始化

    std::cout << "Hello, world2, Init" << std::endl;

    return true;
}

int HelloWorld2::OnPropertyCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar )
{
    //属性回调事件，只要属性值内容有变化，就会被回调
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetInt() << " NewValue: " << newVar.GetInt() << std::endl;

    return 0;
}

bool HelloWorld2::AfterInit()
{
// #ifdef NF_USE_ACTOR
//     if(pPluginManager->GetActorID() == NFIActorManager::EACTOR_MAIN)
// #endif
    {
        std::cout << "Hello, world2, AfterInit" << std::endl;

		//created a object for this test
        NFIObject* pObject = new NFCObject(NFGUID(0, 1), pPluginManager);

		//add a property name is "Hello" of this object
        pObject->GetPropertyManager()->AddProperty(pObject->Self(), "Hello", TDATA_STRING, true, true, true, true, 0, "");
		//add a property name is "World" of this object
        pObject->GetPropertyManager()->AddProperty(pObject->Self(), "World", TDATA_INT, true, true, true, true, 0, "");

		//set the "world" property value as 1111
        pObject->SetPropertyInt("World", 1111);

		//get the "world" property value and printf it
        const int nProperty1 = pObject->GetPropertyInt("World");
        std::cout << "Property World:" << nProperty1 << std::endl;

        //add a call back functin for "world" property
        pObject->AddPropertyCallBack("World", this, &HelloWorld2::OnPropertyCallBackEvent);

		////set the "world" property value as 2222[than the function "HelloWorld2::OnPropertyCallBackEvent" will be called]
        pObject->SetPropertyInt("World", 2222);

		//get the "world" property value and printf it
        const int nProperty2 = pObject->GetPropertyInt("World");
        std::cout << "Property World:" << nProperty2 << std::endl;

    }

    return true;
}

bool HelloWorld2::Execute()
{
    //每帧执行
    //std::cout << "Hello, world2, Execute" << std::endl;

    return true;
}

bool HelloWorld2::BeforeShut()
{
    //反初始化之前
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool HelloWorld2::Shut()
{
    //反初始化
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}
