/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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


#ifndef NF_THREAD_POOL_MODULE_H
#define NF_THREAD_POOL_NF_THREAD_POOL_MODULE_HMANAGER_H

#include <map>
#include <string>
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"
#include "NFComm/NFCore/NFQueue.hpp"


class NFThreadCell
{
public:
	NFThreadCell(NFIThreadPoolModule* p)
	{
		m_pThreadPoolModule = p;
		mThread = NF_SHARE_PTR<std::thread>(NF_NEW std::thread(&NFThreadCell::Execute, this));
	}

	void AddTask(const NFThreadTask& task)
	{
		mTaskList.Push(task);
	}

protected:

	void Execute()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			//pick the first task and do it
			NFThreadTask task;
			if (mTaskList.TryPop(task))
			{
				task.xThreadFunc->operator()(task);

				//repush the result to the main thread
				//and, do we must to tell the result?
				if (task.xEndFunc)
				{
					m_pThreadPoolModule->TaskResult(task);
				}
			}
		}
	}

private:
	NFQueue<NFThreadTask> mTaskList;
	NF_SHARE_PTR<std::thread> mThread;
	NFIThreadPoolModule* m_pThreadPoolModule;
};

class NFThreadPoolModule
    : public NFIThreadPoolModule
{
public:
	NFThreadPoolModule(NFIPluginManager* p);
    virtual ~NFThreadPoolModule();

    virtual bool Init();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Shut();

    virtual bool Execute();

	virtual void DoAsyncTask(const NFGUID taskID, const std::string& data, TASK_PROCESS_FUNCTOR_PTR asyncFunctor, TASK_PROCESS_FUNCTOR_PTR functor_end);

	virtual void TaskResult(const NFThreadTask& task);

protected:
	void ExecuteTaskResult();

private:

	NFQueue<NFThreadTask> mTaskResult;
	
	std::vector<NF_SHARE_PTR<NFThreadCell>> mThreadPool;
	//NFConsistentHashMapEx<int, NFThreadCell> mThreadPool;
};

#endif
