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

#ifndef NFI_THREAD_POOL_MODULE_H
#define NFI_THREAD_POOL_MODULE_H

#include "NFIModule.h"
#include <thread>

///////////////////////////////////////////////////

typedef std::function<std::string(const NFGUID, std::string&)> TASK_PROCESS_FUNCTOR;
typedef std::function<void(const NFGUID, std::string&)> TASK_PROCESS_RESULT_FUNCTOR;


class NFIThreadPoolModule : public NFIModule
{
public:

	void DoAsyncTask(const std::string& data, TASK_PROCESS_FUNCTOR asyncFunctor, TASK_PROCESS_RESULT_FUNCTOR functor_end)
	{
		return DoAsyncTask(0, NFGUID(), data, asyncFunctor, functor_end);
	}

	void DoAsyncTask(const NFGUID taskID, const std::string& data,
		TASK_PROCESS_FUNCTOR asyncFunctor, TASK_PROCESS_RESULT_FUNCTOR functor_end)
	{
		return DoAsyncTask(0, taskID, data,  asyncFunctor, functor_end);
	}

	virtual void DoAsyncTask(const int hash, const NFGUID taskID, const std::string& data,
		TASK_PROCESS_FUNCTOR asyncFunctor, TASK_PROCESS_RESULT_FUNCTOR functor_end) = 0;

	/////repush the result
	virtual void TaskResult(const NFGUID taskID, const std::string& resultData, TASK_PROCESS_RESULT_FUNCTOR functor_end) = 0;
};

#endif
