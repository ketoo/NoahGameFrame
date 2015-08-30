// -------------------------------------------------------------------------
//    @FileName      :    NFCActor.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFCActor
//
// -------------------------------------------------------------------------

#include "NFCActor.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


void NFCActor::HandlerEx( const NFIActorMessage& message, const Theron::Address from )
{
	//添加到主线程事件
	m_pActorManager->GetPluginManager()->HandlerEx(message, from);
}

void NFCActor::AddComponent( NF_SHARE_PTR<NFIComponent> pComponent )
{
	if (nullptr != pComponent)
	{
		m_pComponent = pComponent;

		m_pComponent->Init();
		m_pComponent->AfterInit();
	}
}