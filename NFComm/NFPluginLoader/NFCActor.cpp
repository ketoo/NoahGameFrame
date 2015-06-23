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
	//��ӵ����߳��¼�
	m_pActorManager->GetPluginManager()->HandlerEx(message, from);
}

void NFCActor::RegisterActorModule( const std::string& strModuleName, NFILogicModule* pModule )
{
	mxActorModule.Add(pModule);
}
