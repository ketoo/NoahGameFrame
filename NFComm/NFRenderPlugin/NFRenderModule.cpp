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

#include "NFRenderModule.h"

bool NFRenderModule::Awake()
{
    SetupRenderer();

	return true;
}

bool NFRenderModule::Init()
{
   
	return true;
}

bool NFRenderModule::AfterInit()
{


	return true;
}

bool NFRenderModule::CheckConfig()
{

	return true;
}

bool NFRenderModule::ReadyExecute()
{

	return true;
}

bool NFRenderModule::Execute()
{
	return true;
}

bool NFRenderModule::BeforeShut()
{


	return true;
}

bool NFRenderModule::Shut()
{

	return true;
}

bool NFRenderModule::Finalize()
{


	return true;
}

bool NFRenderModule::OnReloadPlugin()
{

	return true;
}

int NFRenderModule::SetupRenderer()
{
    //mCurrentScene = NF_SHARE_PTR<NFScene>(new NFScene());


    return 0;
}
