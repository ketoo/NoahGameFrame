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
    return true;
}

bool NFCEventModule::AfterInit()
{
    return true;
}

bool NFCEventModule::BeforeShut()
{
    return true;
}

bool NFCEventModule::Shut()
{
    return true;
}

bool NFCEventModule::Execute()
{
    return true;
}

bool NFCEventModule::DoEvent(const NFEventDefine nEventID, const NFIDataList & valueList)
{
	return false;
}

bool NFCEventModule::ExistEventCallBack(const NFEventDefine nEventID)
{
	return false;
}

bool NFCEventModule::RemoveEventCallBack(const NFEventDefine nEventID)
{
	return false;
}

bool NFCEventModule::DoEvent(const NFGUID self, const NFEventDefine nEventID, const NFIDataList & valueList)
{
	return false;
}

bool NFCEventModule::ExistEventCallBack(const NFGUID self, const NFEventDefine nEventID)
{
	return false;
}

bool NFCEventModule::RemoveEventCallBack(const NFGUID self, const NFEventDefine nEventID)
{
	return false;
}

bool NFCEventModule::AddEventCallBack(const NFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb)
{
	return false;
}

bool NFCEventModule::AddEventCallBack(const NFGUID self, const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb)
{
	return false;
}
