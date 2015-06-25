// -------------------------------------------------------------------------
//    @FileName      :    NFCActor.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFCActor
//
// -------------------------------------------------------------------------

#include "NFCActor.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_USE_ACTOR

void NFCActor::HandlerEx( const NFIActorMessage& message, const Theron::Address from )
{
	//添加到主线程事件
	m_pActorManager->GetPluginManager()->HandlerEx(message, from);
}

void NFCActor::RegisterActorComponent( const std::string& strModuleName, NFIComponent* pComponent )
{
	mxActorComponent.Add(pComponent);
}

#endif