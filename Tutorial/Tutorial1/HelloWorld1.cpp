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
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "HelloWorld1.h"

bool NFHelloWorld1::Init()
{
	std::cout << typeid(NFHelloWorld1).name() << std::endl;

    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

    std::cout << "Hello, world1, Init" << std::endl;

    return true;
}

bool NFHelloWorld1::AfterInit()
{
    
    std::cout << "Hello, world1, AfterInit" << std::endl;


	NFDataList dataList;
	dataList.Add("1");
	dataList.AddFloat(2.0f);
	dataList.AddObject(NFGUID(3,3));
	dataList.AddVector2(NFVector2(4.0f, 4.0f));

	for (int i = 0; i < dataList.GetCount(); ++i)
	{
		std::cout << dataList.ToString(i) << std::endl;
	}

    cout<<"===========Record Ini Test Start================="<<endl;
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(NFGUID(0, 10), 1, 0, NFrame::Player::ThisName(), "", NFDataList::Empty());
    
    //读取静态配置表
    int index = pObject->GetRecordInt(NFrame::Player::TestRecordIni::ThisName(),0,NFrame::Player::TestRecordIni::SlotIndex);
    string id = pObject->GetRecordString(NFrame::Player::TestRecordIni::ThisName(),0,NFrame::Player::TestRecordIni::HeroID);
    //获取配置表数量
    int count = pObject->GetRecordValidRowCount(NFrame::Player::TestRecordIni::ThisName());
    string recordName = NFrame::Player::TestRecordIni::ThisName();
    int heroId = NFrame::Player::TestRecordIni::HeroID;
    int equipId = NFrame::Player::TestRecordIni::EquipmentID;
    int slotIndex = NFrame::Player::TestRecordIni::SlotIndex;
    //case 1:读取所有的配置
    cout<<"Case 1:Read All Static Config"<<endl;
    for(int i =0;i<count;i++){
        cout<<"HeroID:"<<pObject->GetRecordString(recordName,i,heroId)<<"  ";
        cout<<"EquipmentID:"<<pObject->GetRecordString(recordName,i,equipId)<<"  ";
        cout<<"SlotIndex:"<<pObject->GetRecordInt(recordName,i,slotIndex)<<"  "<<endl;
    }
    cout<<"\n\nCase 2:Find By value"<<endl;
    //case 2: Find
    NF_SHARE_PTR<NFIRecord> record = pObject->FindRecord(NFrame::Player::TestRecordIni::ThisName());
    string key = "hero7"; 
    NFDataList result;
    record->FindString(heroId,key,result);
    for(int i = 0;i<result.GetCount();i++){
        cout<<"Find resut For"<<key<<" :"<<result.Int(i)<<endl;
    }
    
    cout<<"===========Record Ini Test End================="<<endl;
    return true;
}

bool NFHelloWorld1::Execute()
{
    
    //std::cout << "Hello, world1, Execute" << std::endl;

    return true;
}

bool NFHelloWorld1::BeforeShut()
{
    std::cout << "Hello, world1, BeforeShut1111" << std::endl;

    system("PAUSE");

    
    std::cout << "Hello, world1, BeforeShut" << std::endl;

    return true;
}

bool NFHelloWorld1::Shut()
{
    
    std::cout << "Hello, world1, Shut" << std::endl;

    return true;
}
