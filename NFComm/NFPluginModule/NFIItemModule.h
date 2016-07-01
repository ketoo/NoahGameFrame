// -------------------------------------------------------------------------
//    @FileName         £º    NFIItemModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-07-07
//    @Module           £º    NFIItemModule
//
// -------------------------------------------------------------------------

#ifndef NFI_ITEM_MODULE_H
#define NFI_ITEM_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIItemModule
    : public NFILogicModule,
	public NFMap<int, NFILogicModule>
{
public:
	bool ResgisterConsumeModule(const NFMsg::EItemType eModuleType, NFILogicModule* pModule)
	{
		return true;
	}

	template <typename T>
	T* GetConsumeModule(const NFMsg::EItemType eModuleType)
	{

		return (T*)NULL;
	}
};

#endif