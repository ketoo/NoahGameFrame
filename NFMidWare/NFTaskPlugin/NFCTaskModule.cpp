// -------------------------------------------------------------------------
//    @FileName				:    NFCTaskModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCTaskModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCTaskModule.h"

bool NFCTaskModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    return true;
}

bool NFCTaskModule::Shut()
{
    return true;
}

bool NFCTaskModule::Execute()
{
    return true;
}

bool NFCTaskModule::AfterInit()
{

	
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTaskModule::OnClassObjectEvent );
	
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACCEPT_TASK, this, &NFCTaskModule::OnClientAcceptTask)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_COMPELETE_TASK, this, &NFCTaskModule::OnClientPushTask)) { return false; }

    return true;
}

int NFCTaskModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
    }

    return 0;
}

void NFCTaskModule::OnClientAcceptTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAcceptTask)
}

void NFCTaskModule::OnClientPushTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqCompeleteTask)
}

void NFCTaskModule::OnClientPushCustom(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::cu)
}
