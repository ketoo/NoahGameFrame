#include "HelloWorld2.h"
#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFDataList.hpp"

bool NFCHelloWorld2::Init()
{
    

    std::cout << "Hello, world2, Init" << std::endl;

    return true;
}

int NFCHelloWorld2::OnPropertyCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFData& oldVar, const NFData& newVar)
{
    
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetInt() << " NewValue: " << newVar.GetInt() << std::endl;

    return 0;
}

bool NFCHelloWorld2::AfterInit()
{
	NFDataList xData;
	xData.AddInt(111);




// #ifdef NF_USE_ACTOR
//     if(pPluginManager->GetActorID() == NFIActorManager::EACTOR_MAIN)
// #endif
    {
        std::cout << "Hello, world2, AfterInit" << std::endl;

		//created a object for this test
        NFIObject* pObject = new NFCObject(NFGUID(0, 1), pPluginManager);

		//add a property name is "Hello" of this object
        pObject->GetPropertyManager()->AddProperty(pObject->Self(), "Hello", TDATA_STRING);
		//add a property name is "World" of this object
        pObject->GetPropertyManager()->AddProperty(pObject->Self(), "World", TDATA_INT);

		//set the "world" property value as 1111
        pObject->SetPropertyInt("World", 1111);
		int n1 = pObject->GetPropertyInt32("World");
		//get the "world" property value and printf it
        const int nProperty1 = pObject->GetPropertyInt32("World");
        std::cout << "Property World:" << nProperty1 << std::endl;

        //add a call back functin for "world" property
        pObject->AddPropertyCallBack("World", this, &NFCHelloWorld2::OnPropertyCallBackEvent);

		////set the "world" property value as 2222[than the function "HelloWorld2::OnPropertyCallBackEvent" will be called]
        pObject->SetPropertyInt("World", 2222);
		int n2 = pObject->GetPropertyInt32("World");

		//get the "world" property value and printf it
        const int nProperty2 = pObject->GetPropertyInt32("World");
        std::cout << "Property World:" << nProperty2 << std::endl;

    }

    return true;
}

bool NFCHelloWorld2::Execute()
{
    
    //std::cout << "Hello, world2, Execute" << std::endl;

    return true;
}

bool NFCHelloWorld2::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFCHelloWorld2::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}
