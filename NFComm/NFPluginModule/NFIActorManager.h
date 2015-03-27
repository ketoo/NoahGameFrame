// -------------------------------------------------------------------------
//    @FileName      :    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_ACTOR_MANAGER_H_
#define _NFI_ACTOR_MANAGER_H_

#ifdef NF_USE_ACTOR
#include "Theron/Receiver.h"
#include "Theron/Actor.h"
#include "Theron/Framework.h"
#include "Theron/Detail/Handlers/ReceiverHandler.h"
#endif

#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIActorManager : public NFILogicModule
{
public:
#ifdef NF_USE_ACTOR

	enum EACTOR
	{
		//ģ���б�,kernel�б�--�������ú󣬿��Կ��ܶ࣬Ŀǰû�������ã������Ҫ�����������������û�ռ�úܶ��ڴ�
        EACTOR_MAIN,
		EACTOR_END,
	};

	virtual const Theron::Address GetAddress( EACTOR eActor) = 0;
#else
#endif


};

#endif