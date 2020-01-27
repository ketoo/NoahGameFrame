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

#include "NFErrorBusModule.h"

NFIErrorBusModule::ErrorData NFErrorBusModule::GetLastError()
{
	if (mErrList.size() > 0)
	{
		return mErrList.back();
	}

	return ErrorData();
}

void NFErrorBusModule::AddError(const NFGUID object, const ErrID err, const std::string & stData)
{
	ErrorData xErrData;
	xErrData.data = stData;
	xErrData.errID = err;
	xErrData.id = object;

	mErrList.push_back(xErrData);

	if (mErrList.size() > 1000)
	{
		auto it = mErrList.end();
		std::advance(it, -100);
		mErrList.erase(mErrList.begin(), it);
	}
}

void NFErrorBusModule::ClearAllError()
{
	mErrList.clear();
}

