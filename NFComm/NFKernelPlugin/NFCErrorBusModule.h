// -------------------------------------------------------------------------
//    @FileName         :    NFCErrorBusModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-25---Anzac day....
//    @Module           :    NFCErrorBusModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ERROR_BUS_MODULE_H
#define NFC_ERROR_BUS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIErrorBusModule.h"

class NFCErrorBusModule
    : public NFIErrorBusModule
{
public:
	NFCErrorBusModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCErrorBusModule()
    {
    }

	virtual ErrorData GetLastError();
	
	virtual void AddError(const NFGUID object, const ErrID err, const std::string& stData);

	virtual void ClearAllError();

private:


private:
	std::list<ErrorData> mErrList;
};

#endif