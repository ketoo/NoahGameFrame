// -------------------------------------------------------------------------
//    @FileName         :   NFCTileModule.h
//    @Author           :   Nick Yang
//    @Date             :   2016/03/07
//    @Module           :   NFCTileModule
//
// -------------------------------------------------------------------------
#include "NFCTileModule.h"

bool NFCTileModule::Init()
{
    return true;
}

bool NFCTileModule::Shut()
{
    return true;
}

bool NFCTileModule::Execute()
{
    return true;
}

bool NFCTileModule::AfterInit()
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

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTileModule::OnClassObjectEvent);
    return true;
}

int NFCTileModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eEvent, const NFIDataList& args)
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
            TransferTileDataToRecord(self);
        }
        break;
    default:
        break;
    }

    return 0;
}

void NFCTileModule::SetTileData(const NFGUID& self, const int x, const int y, const int type)
{
     // TODO
}

void NFCTileModule::TransferTileDataToMap(const NFGUID& self)
{

}

void NFCTileModule::TransferTileDataToRecord(const NFGUID& self)
{

}