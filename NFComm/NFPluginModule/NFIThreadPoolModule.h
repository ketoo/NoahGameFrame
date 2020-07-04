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

#ifndef NFI_THREAD_POOL_MODULE_H
#define NFI_THREAD_POOL_MODULE_H

#include "NFIModule.h"
#include <thread>

///////////////////////////////////////////////////
class NFThreadTask;

typedef std::function<void(NFThreadTask&)> TASK_PROCESS_FUNCTOR;

class NFThreadTask
{
public:
	void Reset()
	{
		nTaskID = NFGUID();
		data = "";
		if (xThreadFunc)
		{
			xThreadFunc = TASK_PROCESS_FUNCTOR();
		}

		if (xEndFunc)
		{
			xEndFunc = TASK_PROCESS_FUNCTOR();
		}
	}

	NFGUID nTaskID;
	std::string data;
	TASK_PROCESS_FUNCTOR xThreadFunc;
	TASK_PROCESS_FUNCTOR xEndFunc;
};


class NFIThreadPoolModule : public NFIModule
{
public:
	virtual int GetThreadCount() = 0;

	template<typename BaseType>
	void DoAsyncTask(const NFGUID taskID, const std::string& data, BaseType* pBase, void (BaseType::*handler_begin)(NFThreadTask&))
	{
        TASK_PROCESS_FUNCTOR functor_begin = std::bind(handler_begin, pBase, std::placeholders::_1);

		DoAsyncTask(taskID, data, functor_begin, nullptr);
	}

	template<typename BaseType>
	void DoAsyncTask(const NFGUID taskID, const std::string& data, BaseType* pBase, void (BaseType::*handler_begin)(NFThreadTask&), void (BaseType::*handler_end)(NFThreadTask&))
	{
        TASK_PROCESS_FUNCTOR functor_begin = std::bind(handler_begin, pBase, std::placeholders::_1);
		TASK_PROCESS_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1);

		DoAsyncTask(taskID, data, functor_begin, functor_end);
	}

    void DoAsyncTask(const NFGUID taskID, const std::string& data, TASK_PROCESS_FUNCTOR asyncFunctor)
	{
		TASK_PROCESS_FUNCTOR functor_end;
		DoAsyncTask(taskID, data, asyncFunctor, functor_end);
	}

	virtual void DoAsyncTask(const NFGUID taskID, const std::string& data, TASK_PROCESS_FUNCTOR asyncFunctor, TASK_PROCESS_FUNCTOR functor_end) = 0;

	/////repush the result
	virtual void TaskResult(const NFThreadTask& task) = 0;
};

#endif
