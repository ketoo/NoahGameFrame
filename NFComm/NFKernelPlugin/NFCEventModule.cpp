// -------------------------------------------------------------------------
//    @FileName			:    NFCEventModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventModule
//
// -------------------------------------------------------------------------

#include "NFCEventModule.h"

bool NFCEventModule::Init()
{
	m_pKernelodule = GetPluginManager()->FindModule<NFIKernelModule>();

    return true;
}

bool NFCEventModule::AfterInit()
{
    return true;
}

bool NFCEventModule::BeforeShut()
{	
	mModuleEventInfoMapEx.ClearAll();
	mObjectEventInfoMapEx.ClearAll();

    return true;
}

bool NFCEventModule::Shut()
{
    return true;
}

bool NFCEventModule::Execute()
{
	//remove
	if (mModuleRemoveListEx.Count() > 0)
	{
		NFEventDefine nEventID = NFEventDefine::NFED_UNKNOW;;
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

bool NFCEventModule::DoEvent(const NFEventDefine nEventID, const NFDataList & valueList)
{
	bool bRet = false;

	NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (xEventListPtr)
	{
		MODULE_EVENT_FUNCTOR_PTR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			MODULE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(nEventID, valueList);

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

bool NFCEventModule::ExistEventCallBack(const NFEventDefine nEventID)
{
	return mModuleEventInfoMapEx.ExistElement(nEventID);
}

bool NFCEventModule::RemoveEventCallBack(const NFEventDefine nEventID)
{
	return mModuleEventInfoMapEx.RemoveElement(nEventID);
}

bool NFCEventModule::DoEvent(const NFGUID self, const NFEventDefine nEventID, const NFDataList & valueList)
{
	bool bRet = false;

	if (!m_pKernelodule->ExistObject(self))
	{
		return bRet;
	}

	NF_SHARE_PTR<NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return bRet;
	}

	NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr = xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		return bRet;
	}

	OBJECT_EVENT_FUNCTOR_PTR pFunPtr;
	bool bFunRet = xEventListPtr->First(pFunPtr);
	while (bFunRet)
	{
		OBJECT_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nEventID, valueList);

		bFunRet = xEventListPtr->Next(pFunPtr);
	}

	return bRet;
}

bool NFCEventModule::ExistEventCallBack(const NFGUID self, const NFEventDefine nEventID)
{
	NF_SHARE_PTR<NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->ExistElement(nEventID);
}

bool NFCEventModule::RemoveEventCallBack(const NFGUID self, const NFEventDefine nEventID)
{
	NF_SHARE_PTR<NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->RemoveElement(nEventID);
}

bool NFCEventModule::RemoveEventCallBack(const NFGUID self)
{
	return mObjectEventInfoMapEx.RemoveElement(self);
}

bool NFCEventModule::AddEventCallBack(const NFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb)
{
	NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR_PTR>>(NF_NEW NFList<MODULE_EVENT_FUNCTOR_PTR>());
		mModuleEventInfoMapEx.AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return false;
}

bool NFCEventModule::AddEventCallBack(const NFGUID self, const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb)
{
	if (!m_pKernelodule->ExistObject(self))
	{
		return false;
	}
	
	NF_SHARE_PTR<NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		xEventMapPtr = NF_SHARE_PTR<NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>>(NF_NEW NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>());
		mObjectEventInfoMapEx.AddElement(self, xEventMapPtr);
	}

	NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr =  xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR_PTR>>(NF_NEW NFList<OBJECT_EVENT_FUNCTOR_PTR>());
		xEventMapPtr->AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return true;
}
