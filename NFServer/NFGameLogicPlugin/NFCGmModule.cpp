// -------------------------------------------------------------------------
//    @FileName         :    NFCGmModule.cpp
//    @Author               :    s.Guo
//    @Date                 :    2013-06-11
//    @Module               :    NFCGmModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCGmModule.h"

bool NFCGmModule::Init()
{
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

	assert( NULL != m_pGameServerNet_ServerModule );

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_PROPERTY_INT, this,		&NFCGmModule::OnGMPropertyIntProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_PROPERTY_STR	, this,		&NFCGmModule::OnGMPropertyStrProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_PROPERTY_OBJECT, this,	&NFCGmModule::OnGMPropertyObjectProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_PROPERTY_FLOAT	, this, &NFCGmModule::OnGMPropertyFloatProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_RECORD_INT		, this, &NFCGmModule::OnGMRecordIntProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_RECORD_STR		, this, &NFCGmModule::OnGMRecordStrProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_RECORD_OBJECT	, this,		&NFCGmModule::OnGMRecordObjectProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_RECORD_FLOAT	, this,		&NFCGmModule::OnGMRecordFloatProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CMD_NORMAL			, this, &NFCGmModule::OnGMNormalProcess)){ return false; }
    return true;
}

bool NFCGmModule::Shut()
{
    return true;
}

bool NFCGmModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCGmModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>("NFCLevelModule");
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
    m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>("NFCHeroModule");
    
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
	assert( NULL != m_pLogModule );
    assert(NULL != m_pLevelModule);
    assert(NULL != m_pPackModule);
    assert(NULL != m_pHeroModule);

    return true;
}

void NFCGmModule::OnGMPropertyIntProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strPropertyName ;
	if (xMsg.has_command_str_value())
	{
		strPropertyName = xMsg.command_str_value();
	}

	int nValue =0;
	if (xMsg.has_command_value_int())
	{
		nValue = xMsg.command_value_int();
	}

	if (strPropertyName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}

	m_pKernelModule->SetPropertyInt(nPlayerID, strPropertyName, nValue);
}

void NFCGmModule::OnGMPropertyStrProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strPropertyName ;
	if (xMsg.has_command_str_value())
	{
		strPropertyName = xMsg.command_str_value();
	}

	std::string strValue =0;
	if (xMsg.has_command_value_str())
	{
		strValue = xMsg.command_value_str();
	}

	if (strPropertyName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}

	m_pKernelModule->SetPropertyString(nPlayerID, strPropertyName, strValue);
}

void NFCGmModule::OnGMPropertyObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strPropertyName ;
	if (xMsg.has_command_str_value())
	{
		strPropertyName = xMsg.command_str_value();
	}

	NFGUID nValue;
	if (xMsg.has_command_value_object())
	{
		nValue = NFINetModule::PBToNF(xMsg.command_value_object());
	}

	if (strPropertyName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}

	m_pKernelModule->SetPropertyObject(nPlayerID, strPropertyName, nValue);
}

void NFCGmModule::OnGMPropertyFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strPropertyName ;
	if (xMsg.has_command_str_value())
	{
		strPropertyName = xMsg.command_str_value();
	}

	float fValue =0;
	if (xMsg.has_command_value_float())
	{
		fValue = xMsg.command_value_float();
	}

	if (strPropertyName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}

	m_pKernelModule->SetPropertyFloat(nPlayerID, strPropertyName, fValue);
}

void NFCGmModule::OnGMRecordIntProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strRecordName ;
	if (xMsg.has_command_str_value())
	{
		strRecordName = xMsg.command_str_value();
	}
	
	int nValue =0;
	if (xMsg.has_command_value_int())
	{
		nValue = xMsg.command_value_int();
	}

	int nRow =0;
	if (xMsg.has_row())
	{
		nRow = xMsg.row();
	}

	int ncol =0;
	if (xMsg.has_col())
	{
		ncol = xMsg.col();
	}

	if (strRecordName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}


    CheckAndAddRow(nPlayerID, strRecordName, nRow);

	NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(nPlayerID, strRecordName);
	if (pRecord.get())
	{
		pRecord->SetInt(nRow, ncol, nValue);
	}
}

void NFCGmModule::OnGMRecordStrProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strRecordName ;
	if (xMsg.has_command_str_value())
	{
		strRecordName = xMsg.command_str_value();
	}

	std::string strValue ;
	if (xMsg.has_command_value_str())
	{
		strValue = xMsg.command_value_str();
	}

	int nRow =0;
	if (xMsg.has_row())
	{
		nRow = xMsg.row();
	}

	int ncol =0;
	if (xMsg.has_col())
	{
		ncol = xMsg.col();
	}

	if (strRecordName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}

    CheckAndAddRow(nPlayerID, strRecordName, nRow);
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(nPlayerID, strRecordName);
	if (pRecord.get())
	{
		pRecord->SetString(nRow, ncol, strValue.data());
	}
}

void NFCGmModule::OnGMRecordObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strRecordName ;
	if (xMsg.has_command_str_value())
	{
		strRecordName = xMsg.command_str_value();
	}

	NFGUID nValue;
	if (xMsg.has_command_value_object())
	{
		nValue = NFINetModule::PBToNF(xMsg.command_value_object());
	}

	int nRow =0;
	if (xMsg.has_row())
	{
		nRow = xMsg.row();
	}

	int ncol =0;
	if (xMsg.has_col())
	{
		ncol = xMsg.col();
	}

	if (strRecordName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}

    CheckAndAddRow(nPlayerID, strRecordName, nRow);
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(nPlayerID, strRecordName);
	if (pRecord.get())
	{
		pRecord->SetObject(nRow, ncol, nValue);
	}
}

void NFCGmModule::OnGMRecordFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strRecordName ;
	if (xMsg.has_command_str_value())
	{
		strRecordName = xMsg.command_str_value();
	}

	int nValue =0;
	if (xMsg.has_command_value_float())
	{
		nValue = xMsg.command_value_float();
	}

	int nRow =0;
	if (xMsg.has_row())
	{
		nRow = xMsg.row();
	}

	int ncol =0;
	if (xMsg.has_col())
	{
		ncol = xMsg.col();
	}

	if (strRecordName.empty())
	{
		return;
	}

	if (xMsg.command_id() != NFMsg::ReqCommand::EGCT_MODIY_PROPERTY)
	{
		return;
	}

    CheckAndAddRow(nPlayerID, strRecordName, nRow);
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(nPlayerID, strRecordName);
	if (pRecord.get())
	{
		pRecord->SetInt(nRow, ncol, nValue);
	}
}

void NFCGmModule::OnGMNormalProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCommand);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
	{
		return;
	}

	std::string strPropertyName ;
	if (xMsg.has_command_str_value())
	{
		strPropertyName = xMsg.command_str_value();
	}

	NFMsg::ReqCommand::EGameCommandType eCmdID = xMsg.command_id();
	switch (eCmdID)
	{
	case NFMsg::ReqCommand::EGCT_ADD_ROLE_EXP:
		{
			if (xMsg.has_command_value_int())
			{
				int nValue  = xMsg.command_value_int();
				m_pLevelModule->AddExp(nPlayerID, nValue);
			}
		}
		break;

    case NFMsg::ReqCommand::EGCT_MODIY_ITEM:
    {
        int nCount = 0;
        if (xMsg.has_command_value_int())
        {
            nCount = xMsg.command_value_int();
        }

        string strItemID;
        if (xMsg.has_command_str_value())
        {
            strItemID = xMsg.command_str_value();
        }

        if (strItemID.empty() || nCount <=0)
        {
            break;
        }


        const int nItemType = m_pElementInfoModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
        switch (nItemType)
        {
        case NFMsg::EIT_EQUIP:
        {
            m_pPackModule->CreateEquip(nPlayerID, strItemID);
        }
        break;
        case NFMsg::EIT_HERO_CARD:
        {
            m_pHeroModule->AddHero(nPlayerID, strItemID);
        }
        break;
        default:
            m_pPackModule->CreateItem(nPlayerID, strItemID, nCount);
            break;
        }

    }
    break;
	default:
		break;
	}
}

void NFCGmModule::CheckAndAddRow(const NFGUID& self, const std::string strRecordName, const int nRow)
{

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (pRecord.get())
    {
        if (!pRecord->IsUsed(nRow))
        {
            NFCDataList varData = pRecord->GetInitData();

            pRecord->AddRow(nRow, varData);
        }
    }
}
