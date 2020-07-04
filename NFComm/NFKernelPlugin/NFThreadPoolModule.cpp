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


#include "NFThreadPoolModule.h"

NFThreadPoolModule::NFThreadPoolModule(NFIPluginManager* p)
{
    m_bIsExecute = true;
	pPluginManager = p;
}

NFThreadPoolModule::~NFThreadPoolModule()
{
}

bool NFThreadPoolModule::Init()
{
	for (int i = 0; i < pPluginManager->GetAppCPUCount(); ++i)
	{
		mThreadPool.push_back(NF_SHARE_PTR<NFThreadCell>(NF_NEW NFThreadCell(this)));
	}

    return true;
}

bool NFThreadPoolModule::AfterInit()
{

    return true;
}

bool NFThreadPoolModule::BeforeShut()
{
    return true;
}

bool NFThreadPoolModule::Shut()
{
 
    return true;
}

bool NFThreadPoolModule::Execute()
{
	ExecuteTaskResult();

    return true;
}

void NFThreadPoolModule::DoAsyncTask(const NFGUID taskID, const std::string & data, TASK_PROCESS_FUNCTOR asyncFunctor, TASK_PROCESS_FUNCTOR functor_end)
{
	NFThreadTask task;
	task.nTaskID = taskID;
	task.data = data;
	task.xThreadFunc = asyncFunctor;
	task.xEndFunc = functor_end;
	
	int index = 0;
	if (!taskID.IsNull())
	{
		index = taskID.nData64 % mThreadPool.size();
	}
	
	NF_SHARE_PTR<NFThreadCell> threadobject = mThreadPool[index];
	threadobject->AddTask(task);
}

void NFThreadPoolModule::ExecuteTaskResult()
{
	NFThreadTask xMsg;
	while (mTaskResult.TryPop(xMsg))
	{
		if (xMsg.xEndFunc)
		{
			xMsg.xEndFunc.operator()(xMsg);
		}
	}
}

void NFThreadPoolModule::TaskResult(const NFThreadTask& task)
{
	mTaskResult.Push(task);
}

int NFThreadPoolModule::GetThreadCount()
{
	return mThreadPool.size();
}

