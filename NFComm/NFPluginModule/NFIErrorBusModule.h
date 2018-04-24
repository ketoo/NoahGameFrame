// -------------------------------------------------------------------------
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-25
//    @Module           :    NFIErrorBusModule
//    @Desc             :
// -------------------------------------------------------------------------


#ifndef NFI_ERROR_BUS_MODULE_H
#define NFI_ERROR_BUS_MODULE_H

#include <iostream>
#include "NFGUID.h"
#include "NFIModule.h"


class NFIErrorBusModule
    : public NFIModule
{

public:
	enum ErrID
	{
		ERR_NONE,
	};

	struct ErrorData
	{
		ErrorData()
		{
			errID = ErrID::ERR_NONE;
		}

		NFGUID id;
		ErrID errID;
		std::string data;
	};

	virtual ErrorData GetLastError() = 0;

	virtual void AddError(const NFGUID object, const ErrID err, const std::string& stData) = 0;

	virtual void ClearAllError() = 0;
};

#endif