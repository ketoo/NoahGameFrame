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

#include "NFEventModule.h"

bool NFEventModule::Init()
{
	m_pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>();

    return true;
}

bool NFEventModule::AfterInit()
{
    return true;
}

bool NFEventModule::BeforeShut()
{	
	mModuleEventInfoMapEx.ClearAll();
	mObjectEventInfoMapEx.ClearAll();

    return true;
}

bool NFEventModule::Shut()
{
    return true;
}

bool NFEventModule::Execute()
{
	//remove
	if (mModuleRemoveListEx.Count() > 0)
	{
		int eventID = 0;
		bool bRet = mModuleRemoveListEx.First(eventID);
		while (bRet)
		{
			mModuleEventInfoMapEx.RemoveElement(eventID);

			bRet = mModuleRemoveListEx.Next(eventID);
		}
	}

	//////
	if (mObjectRemoveListEx.Count() > 0)
	{
		NFGUID xObject;
		bool bRet = mObjectRemoveListEx.First(xObject);
		while (bRet)
		{
			mObjectEventInfoMapEx.RemoveElement(xObject);

			bRet = mObjectRemoveListEx.Next(xObject);
		}
	}

    return true;

}

bool NFEventModule::DoEvent(const int eventID, const NFDataList & valueList)
{
	bool bRet = false;

	auto xEventListPtr = mModuleEventInfoMapEx.GetElement(eventID);
	if (xEventListPtr)
	{
		MODULE_EVENT_FUNCTOR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			pFunPtr.operator()(eventID, valueList);

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

bool NFEventModule::ExistEventCallBack(const int eventID)
{
	return mModuleEventInfoMapEx.ExistElement(eventID);
}

bool NFEventModule::RemoveEventCallBack(const int eventID)
{
	return mModuleEventInfoMapEx.RemoveElement(eventID);
}

bool NFEventModule::DoEvent(const NFGUID self, const int eventID, const NFDataList & valueList)
{
	{
		//for common event call back
		OBJECT_EVENT_FUNCTOR pFunPtr;
		bool bFunRet = mCommonEventInfoMapEx.First(pFunPtr);
		while (bFunRet)
		{
			pFunPtr.operator()(self, eventID, valueList);

			bFunRet = mCommonEventInfoMapEx.Next(pFunPtr);
		}
	}

	bool bRet = false;

	if (!m_pKernelModule->ExistObject(self))
	{
		return bRet;
	}

	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return bRet;
	}

	auto xEventListPtr = xEventMapPtr->GetElement(eventID);
	if (!xEventListPtr)
	{
		return bRet;
	}

	OBJECT_EVENT_FUNCTOR pFunPtr;
	bool bFunRet = xEventListPtr->First(pFunPtr);
	while (bFunRet)
	{
		pFunPtr.operator()(self, eventID, valueList);

		bFunRet = xEventListPtr->Next(pFunPtr);
	}

	return bRet;
}

bool NFEventModule::ExistEventCallBack(const NFGUID self, const int eventID)
{
	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->ExistElement(eventID);
}

bool NFEventModule::RemoveEventCallBack(const NFGUID self, const int eventID)
{
	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->RemoveElement(eventID);
}

bool NFEventModule::RemoveEventCallBack(const NFGUID self)
{
	return mObjectEventInfoMapEx.RemoveElement(self);
}

bool NFEventModule::AddEventCallBack(const int eventID, const MODULE_EVENT_FUNCTOR cb)
{
	auto xEventListPtr = mModuleEventInfoMapEx.GetElement(eventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR>>(NF_NEW NFList<MODULE_EVENT_FUNCTOR>());
		mModuleEventInfoMapEx.AddElement(eventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return false;
}

bool NFEventModule::AddEventCallBack(const NFGUID self, const int eventID, const OBJECT_EVENT_FUNCTOR cb)
{
	if (!m_pKernelModule->ExistObject(self))
	{
		return false;
	}
	
	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		xEventMapPtr = NF_SHARE_PTR<NFMapEx<int, NFList<OBJECT_EVENT_FUNCTOR>>>(NF_NEW NFMapEx<int, NFList<OBJECT_EVENT_FUNCTOR>>());
		mObjectEventInfoMapEx.AddElement(self, xEventMapPtr);
	}

	auto xEventListPtr =  xEventMapPtr->GetElement(eventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR>>(NF_NEW NFList<OBJECT_EVENT_FUNCTOR>());
		xEventMapPtr->AddElement(eventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return true;
}

bool NFEventModule::AddCommonEventCallBack(const OBJECT_EVENT_FUNCTOR cb)
{
	mCommonEventInfoMapEx.Add(cb);
	return true;
}
