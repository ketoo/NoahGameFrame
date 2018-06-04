// -------------------------------------------------------------------------
//    @FileName				:    NFCTestModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCTestModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCTestModule.h"

bool NFCTestModule::Init()
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

bool NFCTestModule::Shut()
{
    return true;
}

bool NFCTestModule::Execute()
{
    return true;
}

bool NFCTestModule::AfterInit()
{

	
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTestModule::OnClassObjectEvent );
	
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACCEPT_TASK, this, &NFCTestModule::OnClientAcceptTask)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_COMPELETE_TASK, this, &NFCTestModule::OnClientPushTask)) { return false; }

    return true;
}

int NFCTestModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
    }

    return 0;
}

void NFCTestModule::OnClientAcceptTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAcceptTask)
}

void NFCTestModule::OnClientPushTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqCompeleteTask)
}