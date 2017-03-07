#ifndef NF_COROUTINE_MODULE_H
#define NF_COROUTINE_MODULE_H

#include "NFComm/NFPluginModule/NFICoroutineModule.h"
#include <unordered_map>
#include <type_traits>
// -------------------------------------------------------------------------
//    @FileName			:    NFCCoroutineModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCCoroutineModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFContextData.hpp"

class NFCCoroutineModule
	: public NFICoroutineModule
{
public:
	NFCCoroutineModule(NFIPluginManager* p);
	virtual ~NFCCoroutineModule();

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

public:
private:
	NFMapEx<NFINT64, NF_SHARE_PTR<NFContextData>> m_taskMap;
};


#endif
