// -------------------------------------------------------------------------
//    @FileName			:    NFCAsyNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCAsyNoSqlModule
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCAsyNoSqlModule.h"

bool NFCNoSqlComponent::Init()
{
	return false;
}

bool NFCNoSqlComponent::AfterInit()
{
	return false;
}

int NFCNoSqlComponent::OnASyncEvent(const NFGUID & self, const int event, std::string & arg)
{
	int nRet = 0;
	switch (event)
	{

	default:
		break;
	}

	return nRet;
}

NF_SHARE_PTR<NFIComponent> NFCNoSqlComponent::CreateNewInstance()
{
	return  NF_SHARE_PTR<NFIComponent>(NF_NEW  NFCNoSqlComponent(NFGUID(1, 2), ""));
}

/////////////////////////////////////////////

NFCAsyNoSqlModule::NFCAsyNoSqlModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCAsyNoSqlModule::~NFCAsyNoSqlModule()
{

}

bool NFCAsyNoSqlModule::Init()
{

	return true;
}

bool NFCAsyNoSqlModule::Shut()
{

	return true;
}

bool NFCAsyNoSqlModule::AfterInit()
{
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();

	StartActorPool(10);
	return true;
}

bool NFCAsyNoSqlModule::StartActorPool(const int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		int nActorID = m_pActorModule->RequireActor<NFCNoSqlComponent>(this, &NFCAsyNoSqlModule::RequestAsyEnd);
		if (nActorID > 0)
		{
			mActorList.AddElement(i, NF_SHARE_PTR<int>(NF_NEW int(nActorID)));
		}
	}

	return true;
}

bool NFCAsyNoSqlModule::CloseActorPool()
{
	return false;
}

int NFCAsyNoSqlModule::RequestAsyEnd(const NFGUID & self, const int nFormActor, const int nEventID, const std::string & strData)
{
	return 0;
}

bool NFCAsyNoSqlModule::Execute()
{
	

	return true;
}
