// -------------------------------------------------------------------------
//    @FileName         :   NFCTeamModule.h
//    @Author           :   flyicegood
//    @Date             :   2016/03/07
//    @Module           :   NFCTeamModule
//
// -------------------------------------------------------------------------
#include "NFCTeamModule.h"

bool NFCTeamModule::Init()
{
    return true;
}

bool NFCTeamModule::Shut()
{
    return true;
}

bool NFCTeamModule::Execute()
{
    return true;
}

bool NFCTeamModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    
    assert( NULL != m_pKernelModule );
	assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pGameServerNet_ServerModule);
    assert( NULL != m_pUUIDModule);
    assert( NULL != m_pElementInfoModule);

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTeamModule::OnClassObjectEvent);
    return true;
}

int NFCTeamModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eEvent, const NFIDataList& args)
{
    switch (eEvent)
    {
    case COE_CREATE_HASDATA:
        {
            // fill record data into map
            TransferTileDataToMap(self);
        }
        break;
    case COE_CREATE_FINISH:
        {
            // some trees grown up offline
        }
        break;
    case COE_BEFOREDESTROY:
        {
        }
        break;
    default:
        break;
    }

    return 0;
}

bool NFCTeamModule::CreateTeam(const NFGUID& self, const std::string& strName, NFGUID& xTeam)
{

}
