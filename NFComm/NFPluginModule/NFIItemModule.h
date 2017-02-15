// -------------------------------------------------------------------------
//    @FileName				:    NFCItemModule.h
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCItemModule
//    @Desc                 :
// -------------------------------------------------------------------------

#ifndef NFI_ITEM_MODULE_H
#define NFI_ITEM_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIItemModule
    : public NFIModule,
	public NFMap<int, NFIModule>
{
public:
	bool ResgisterConsumeModule(const NFMsg::EItemType eModuleType, NFIModule* pModule)
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