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
	m_pKernelodule = GetPluginManager()->FindModule<NFIKernelModule>();

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
		int nEventID = 0;
		bool bRet = mModuleRemoveListEx.First(nEventID);
		while (bRet)
		{
			mModuleEventInfoMapEx.RemoveElement(nEventID);

			bRet = mModuleRemoveListEx.Next(nEventID);
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

bool NFEventModule::DoEvent(const int nEventID, const NFDataList & valueList)
{
	bool bRet = false;

	auto xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (xEventListPtr)
	{
		MODULE_EVENT_FUNCTOR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			pFunPtr.operator()(nEventID, valueList);

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

bool NFEventModule::ExistEventCallBack(const int nEventID)
{
	return mModuleEventInfoMapEx.ExistElement(nEventID);
}

bool NFEventModule::RemoveEventCallBack(const int nEventID)
{
	return mModuleEventInfoMapEx.RemoveElement(nEventID);
}

bool NFEventModule::DoEvent(const NFGUID self, const int nEventID, const NFDataList & valueList)
{
	{
		//for common event call back
		OBJECT_EVENT_FUNCTOR pFunPtr;
		bool bFunRet = mCommonEventInfoMapEx.First(pFunPtr);
		while (bFunRet)
		{
			pFunPtr.operator()(self, nEventID, valueList);

			bFunRet = mCommonEventInfoMapEx.Next(pFunPtr);
		}
	}

	bool bRet = false;

	if (!m_pKernelodule->ExistObject(self))
	{
		return bRet;
	}

	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return bRet;
	}

	auto xEventListPtr = xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		return bRet;
	}

	OBJECT_EVENT_FUNCTOR pFunPtr;
	bool bFunRet = xEventListPtr->First(pFunPtr);
	while (bFunRet)
	{
		pFunPtr.operator()(self, nEventID, valueList);

		bFunRet = xEventListPtr->Next(pFunPtr);
	}

	return bRet;
}

bool NFEventModule::ExistEventCallBack(const NFGUID self, const int nEventID)
{
	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->ExistElement(nEventID);
}

bool NFEventModule::RemoveEventCallBack(const NFGUID self, const int nEventID)
{
	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->RemoveElement(nEventID);
}

bool NFEventModule::RemoveEventCallBack(const NFGUID self)
{
	return mObjectEventInfoMapEx.RemoveElement(self);
}

bool NFEventModule::AddEventCallBack(const int nEventID, const MODULE_EVENT_FUNCTOR cb)
{
	auto xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR>>(NF_NEW NFList<MODULE_EVENT_FUNCTOR>());
		mModuleEventInfoMapEx.AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return false;
}

bool NFEventModule::AddEventCallBack(const NFGUID self, const int nEventID, const OBJECT_EVENT_FUNCTOR cb)
{
	if (!m_pKernelodule->ExistObject(self))
	{
		return false;
	}
	
	auto xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		xEventMapPtr = NF_SHARE_PTR<NFMapEx<int, NFList<OBJECT_EVENT_FUNCTOR>>>(NF_NEW NFMapEx<int, NFList<OBJECT_EVENT_FUNCTOR>>());
		mObjectEventInfoMapEx.AddElement(self, xEventMapPtr);
	}

	auto xEventListPtr =  xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR>>(NF_NEW NFList<OBJECT_EVENT_FUNCTOR>());
		xEventMapPtr->AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return true;
}

bool NFEventModule::AddCommonEventCallBack(const OBJECT_EVENT_FUNCTOR cb)
{
	mCommonEventInfoMapEx.Add(cb);
	return true;
}
