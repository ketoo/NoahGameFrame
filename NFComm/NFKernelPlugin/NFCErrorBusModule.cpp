// -------------------------------------------------------------------------
//    @FileName			:    NFCErrorBusModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-25---Anzac day....
//    @Module           :    NFCErrorBusModule
//
// -------------------------------------------------------------------------

#include "NFCErrorBusModule.h"

NFIErrorBusModule::ErrorData NFCErrorBusModule::GetLastError()
{
	if (mErrList.size() > 0)
	{
		return mErrList.back();
	}

	return ErrorData();
}

void NFCErrorBusModule::AddError(const NFGUID object, const ErrID err, const std::string & stData)
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

void NFCErrorBusModule::ClearAllError()
{
	mErrList.clear();
}

