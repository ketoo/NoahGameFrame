/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "HelloWorld2.h"
#include "NFComm/NFCore/NFObject.h"
#include "NFComm/NFCore/NFDataList.hpp"

bool NFHelloWorld2::Init()
{
    

    std::cout << "Hello, world2, Init" << std::endl;

    return true;
}

int NFHelloWorld2::OnPropertyCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFData& oldVar, const NFData& newVar)
{
    
    std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetInt() << " NewValue: " << newVar.GetInt() << std::endl;

    return 0;
}

bool NFHelloWorld2::AfterInit()
{
	NFDataList xData;
	xData.AddInt(111);

    std::cout << "Hello, world2, AfterInit" << std::endl;

	//created a object for this test
    NFIObject* pObject = new NFObject(NFGUID(0, 1), pPluginManager);

	//add a property name is "Hello" for this object
    pObject->GetPropertyManager()->AddProperty(pObject->Self(), "Hello", TDATA_STRING);
	//add a property name is "World" for this object
    pObject->GetPropertyManager()->AddProperty(pObject->Self(), "World", TDATA_INT);

	//set the "world" property value as 1111
    pObject->SetPropertyInt("World", 1111);
	int n1 = pObject->GetPropertyInt32("World");

	//get the "world" property value and printf it
    std::cout << "Property World:" << n1 << std::endl;

    const int nProperty1 = pObject->GetPropertyInt32("World");
    std::cout << "Property World:" << nProperty1 << std::endl;

    //add a call back functin for "world" property
    pObject->AddPropertyCallBack("World", this, &NFHelloWorld2::OnPropertyCallBackEvent);

	////set the "world" property value as 2222[than the function "HelloWorld2::OnPropertyCallBackEvent" will be called]
    pObject->SetPropertyInt("World", 2222);

	//get the "world" property value and printf it
	int n2 = pObject->GetPropertyInt32("World");
    std::cout << "Property World:" << n2 << std::endl;

    const int nProperty2 = pObject->GetPropertyInt32("World");
    std::cout << "Property World:" << nProperty2 << std::endl;

    return true;
}

bool NFHelloWorld2::Execute()
{
    
    //std::cout << "Hello, world2, Execute" << std::endl;

    return true;
}

bool NFHelloWorld2::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFHelloWorld2::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}
