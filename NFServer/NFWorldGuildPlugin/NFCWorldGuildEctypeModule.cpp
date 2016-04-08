#include "NFCWorldGuildEctypeModule.h"

bool NFCWorldGuildEctypeModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pWorldGuildDataModule = pPluginManager->FindModule<NFIWorldGuildDataModule>("NFCWorldGuildDataModule");
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>("NFCWorldNet_ServerModule");
	m_pWorldGuildModule = pPluginManager->FindModule<NFIWorldGuildModule>("NFCWorldGuildModule");

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pWorldGuildDataModule);
	assert(NULL != m_pWorldNet_ServerModule);
	assert(NULL != m_pWorldGuildModule);

	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_GUILDECTYPEINFO, this, &NFCWorldGuildEctypeModule::OnReqGuildEctypeProcess))
	{
		//Log
	}

	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_SET_GUILDECTYPEINFO, this, &NFCWorldGuildEctypeModule::OnReqGuildEctypeProcess))
	{
		//log
	}

	return true;
}

bool NFCWorldGuildEctypeModule::Shut()
{

	return true;
}

bool NFCWorldGuildEctypeModule::Execute()
{

	return true;
}

bool NFCWorldGuildEctypeModule::AfterInit()
{
	m_pKernelModule->ResgisterCommonPropertyEvent( this, &NFCWorldGuildEctypeModule::OnGuildObjectPropertyCommonEvent );
	m_pKernelModule->ResgisterCommonRecordEvent( this, &NFCWorldGuildEctypeModule::OnGuildObjectRecordCommonEvent );
	m_pKernelModule->AddClassCallBack(NFrame::Guild::ThisName(), this, &NFCWorldGuildEctypeModule::OnObjectClassEvent);
	return true;
}

int NFCWorldGuildEctypeModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
	if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
	{
	}
	else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Guild::EctypServer(), this, &NFCWorldGuildEctypeModule::OnObjectEctypeServerEvent);
	}

	return 0;
}

int NFCWorldGuildEctypeModule::OnObjectEctypeServerEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar )
{
	const NFINT64 nOldValue = oldVar.GetInt();
	const NFINT64 nNewValue = newVar.GetInt();

	if (nOldValue > 0 )
	{
		OnGuildObjectLeave(self, nOldValue);
	}

	if (nNewValue > 0)
	{
		OnGuildObjectEnter(self, nNewValue);
		OnGuildObjectPropertyEnter(self, nNewValue);
		OnGuildObjectRecordEnter(self, nNewValue);
	}

	return 0;
}
void NFCWorldGuildEctypeModule::OnReqGuildEctypeProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqGuildEctypeInfo xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen,xData, nPlayerID))
	{
		return ;
	}

	NFGUID xGuild = NFINetModule::PBToNF(xData.guild_id());
	NF_SHARE_PTR<NFIObject> pGuild = m_pWorldGuildDataModule->GetGuild(xGuild);
	if(nullptr == pGuild)
	{
		return ;
	}

	const int nServerID = pGuild->GetPropertyInt(NFrame::Guild::EctypServer());
	const int nEctypeID = pGuild->GetPropertyInt(NFrame::Guild::EctypID());
	const int nGroupID = pGuild->GetPropertyInt(NFrame::Guild::EctypIDGroup());

	NFMsg::AckGuildEctypeInfo xAck;
	*xAck.mutable_selfid() = xData.selfid();
	*xAck.mutable_xectypeinfo()->mutable_guild_id()= xData.guild_id();
	xAck.mutable_xectypeinfo()->set_server_id(nServerID);
	xAck.mutable_xectypeinfo()->set_ectype_id(nEctypeID);
	xAck.mutable_xectypeinfo()->set_ectype_group(nGroupID);

	m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_GUILDECTYPEINFO, xAck, nSockIndex, nPlayerID);

	return ;
}

void NFCWorldGuildEctypeModule::OnSetGuildEctypeInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::SetGuildEctypeInfo xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen,xData, nPlayerID))
	{
		return ;
	}

	NFGUID xGuild = NFINetModule::PBToNF(xData.xectypeinfo().guild_id());
	NF_SHARE_PTR<NFIObject> pGuild = m_pWorldGuildDataModule->GetGuild(xGuild);
	if(nullptr == pGuild)
	{
		return ;
	}

	pGuild->SetPropertyInt(NFrame::Guild::EctypServer(), xData.xectypeinfo().server_id());
	pGuild->SetPropertyInt(NFrame::Guild::EctypID(), xData.xectypeinfo().ectype_id());
	pGuild->SetPropertyInt(NFrame::Guild::EctypIDGroup(), xData.xectypeinfo().ectype_group());

	const int nServerID = pGuild->GetPropertyInt(NFrame::Guild::EctypServer());
	const int nEctypeID = pGuild->GetPropertyInt(NFrame::Guild::EctypID());
	const int nGroupID = pGuild->GetPropertyInt(NFrame::Guild::EctypIDGroup());

	NFMsg::AckGuildEctypeInfo xAck;
	*xAck.mutable_selfid() = xData.selfid();
	*xAck.mutable_xectypeinfo()->mutable_guild_id()= xData.xectypeinfo().guild_id();
	xAck.mutable_xectypeinfo()->set_server_id(nServerID);
	xAck.mutable_xectypeinfo()->set_ectype_id(nEctypeID);
	xAck.mutable_xectypeinfo()->set_ectype_group(nGroupID);

	m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_GUILDECTYPEINFO, xAck, nSockIndex, nPlayerID);

	return ;
}

int NFCWorldGuildEctypeModule::OnGuildObjectEnter(const NFGUID& xGuid, const int nServerID)
{
	if (xGuid.IsNull() || nServerID <= 0)
	{
		return 1;
	}

	NFMsg::AckPlayerEntryList xPlayerEntryInfoList;

	NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
	*(pEntryInfo->mutable_object_guid()) = NFINetModule::NFToPB(xGuid);

	pEntryInfo->set_x( m_pKernelModule->GetPropertyFloat( xGuid, "X" ) );
	pEntryInfo->set_y( m_pKernelModule->GetPropertyFloat( xGuid, "Y" ) );
	pEntryInfo->set_z( m_pKernelModule->GetPropertyFloat( xGuid, "Z" ) );
	pEntryInfo->set_career_type( m_pKernelModule->GetPropertyInt( xGuid, NFrame::Guild::EctypIDGroup() ) );
	pEntryInfo->set_player_state( m_pKernelModule->GetPropertyInt( xGuid, "State" ) );
	pEntryInfo->set_config_id( m_pKernelModule->GetPropertyString( xGuid, "ConfigID" ) );
	pEntryInfo->set_scene_id( m_pKernelModule->GetPropertyInt( xGuid, NFrame::Guild::EctypID() ) );
	pEntryInfo->set_class_id( m_pKernelModule->GetPropertyString( xGuid, "ClassName" ) );

	if (xPlayerEntryInfoList.object_list_size() <= 0)
	{
		return 0;
	}

	m_pWorldNet_ServerModule->SendMsgToGame(nServerID, NFMsg::EGMI_ACK_CLONE_OBJECT_ENTRY, xPlayerEntryInfoList, xGuid);

	return 1;
}

int NFCWorldGuildEctypeModule::OnGuildObjectLeave(const NFGUID& xGuid, const int nServerID)
{
	if (xGuid.IsNull() || nServerID <= 0)
	{
		return 1;
	}

	NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
	NFMsg::Ident* pIdent = xPlayerLeaveInfoList.add_object_list();
	*pIdent = NFINetModule::NFToPB(xGuid);

	m_pWorldNet_ServerModule->SendMsgToGame(nServerID, NFMsg::EGMI_ACK_CLONE_OBJECT_LEAVE, xPlayerLeaveInfoList, xGuid);

	return 1;
}

int NFCWorldGuildEctypeModule::OnGuildObjectPropertyEnter(const NFGUID& xGuid, const int nServerID)
{
	if (xGuid.IsNull() || nServerID <= 0)
	{
		return 1;
	}

	NFMsg::MultiObjectPropertyList xPrivateMsg;
	NF_SHARE_PTR<NFIObject> pObject = m_pWorldGuildDataModule->GetGuild( xGuid );
	if ( pObject.get() )
	{
		NFMsg::ObjectPropertyList* pPrivateData = xPrivateMsg.add_multi_player_property();

		*(pPrivateData->mutable_player_id()) = NFINetModule::NFToPB(xGuid);

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIProperty> pPropertyInfo = pPropertyManager->First();
		while ( pPropertyInfo.get() )
		{
			if ( pPropertyInfo->Changed() )
			{
				switch ( pPropertyInfo->GetType())
				{
				case TDATA_INT:
					{
						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyInt* pDataInt = pPrivateData->add_property_int_list();
							pDataInt->set_property_name( pPropertyInfo->GetKey() );
							pDataInt->set_data( pPropertyInfo->GetInt() );
						}
					}
					break;

				case TDATA_FLOAT:
					{
						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyFloat* pDataFloat = pPrivateData->add_property_float_list();
							pDataFloat->set_property_name( pPropertyInfo->GetKey() );
							pDataFloat->set_data( pPropertyInfo->GetFloat() );
						}
					}
					break;

				case TDATA_STRING:
					{
						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyString* pDataString = pPrivateData->add_property_string_list();
							pDataString->set_property_name( pPropertyInfo->GetKey() );
							pDataString->set_data( pPropertyInfo->GetString() );
						}
					}
					break;

				case TDATA_OBJECT:
					{
						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyObject* pDataObject = pPrivateData->add_property_object_list();
							pDataObject->set_property_name( pPropertyInfo->GetKey() );
							*(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetObject());
						}
					}
					break;

				default:
					break;
				}
			}

			pPropertyInfo = pPropertyManager->Next();
		}

		m_pWorldNet_ServerModule->SendMsgToGame(nServerID, NFMsg::EGMI_ACK_CLONE_OBJECT_PROPERTY_ENTRY, xPrivateMsg, xGuid);
	}

	return 0;
}

int NFCWorldGuildEctypeModule::OnGuildObjectRecordEnter(const NFGUID& xGuid, const int nServerID)
{
	if (xGuid.IsNull() || nServerID <= 0)
	{
		return 1;
	}

	NFMsg::MultiObjectRecordList xPrivateMsg;

	NF_SHARE_PTR<NFIObject> pObject = m_pWorldGuildDataModule->GetGuild( xGuid );
	if ( pObject.get() )
	{
		NFMsg::ObjectRecordList* pPrivateData = NULL;

		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();
		NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
		while ( pRecord.get() )
		{
			if (!pRecord->GetPublic() && !pRecord->GetPrivate())
			{
				pRecord = pRecordManager->Next();
				continue;
			}

			NFMsg::ObjectRecordBase* pPrivateRecordBase = NULL;
			if (pRecord->GetPrivate())
			{
				if (!pPrivateData)
				{
					pPrivateData = xPrivateMsg.add_multi_player_record();
					*(pPrivateData->mutable_player_id()) = NFINetModule::NFToPB(xGuid);
				}
				pPrivateRecordBase = pPrivateData->add_record_list();
				pPrivateRecordBase->set_record_name(pRecord->GetName());

				m_pWorldNet_ServerModule->OnRecordEnterPack(pRecord, pPrivateRecordBase);
			}

			pRecord = pRecordManager->Next();
		}

		if (xPrivateMsg.multi_player_record_size() > 0)
		{
			m_pWorldNet_ServerModule->SendMsgToGame(nServerID, NFMsg::EGMI_ACK_CLONE_OBJECT_RECORD_ENTRY, xPrivateMsg, xGuid);
		}
	}

	return 0;
}

int NFCWorldGuildEctypeModule::OnGuildObjectPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	if ( "Guild" != m_pKernelModule->GetPropertyString( self, "ClassName" ))
	{
		return 0;
	}

	NFCDataList valueBroadCaseList;
	NFCDataList valueBroadCaseGameList;

	const int nServerID = m_pKernelModule->GetPropertyInt(self, NFrame::Guild::EctypServer());
	if (nServerID <= 0)
	{
		return 1;
	}

	valueBroadCaseList << self;
	valueBroadCaseGameList << nServerID;

	if ( valueBroadCaseList.GetCount() <= 0 || valueBroadCaseList.GetCount() != valueBroadCaseGameList.GetCount())
	{
		return 0;
	}

	switch ( oldVar.GetType() )
	{
	case TDATA_INT:
		{
			NFMsg::ObjectPropertyInt xPropertyInt;
			NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
			*pIdent = NFINetModule::NFToPB(self);

			NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
			pDataInt->set_property_name( strPropertyName );
			pDataInt->set_data( newVar.GetInt() );

			m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_PROPERTY_INT, xPropertyInt);

		}
		break;

	case TDATA_FLOAT:
		{
			NFMsg::ObjectPropertyFloat xPropertyFloat;
			NFMsg::Ident* pIdent = xPropertyFloat.mutable_player_id();
			*pIdent = NFINetModule::NFToPB(self);

			NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
			pDataFloat->set_property_name( strPropertyName );
			pDataFloat->set_data( newVar.GetFloat() );

			m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_PROPERTY_DOUBLE, xPropertyFloat);
		}
		break;

	case TDATA_STRING:
		{
			NFMsg::ObjectPropertyString xPropertyString;
			NFMsg::Ident* pIdent = xPropertyString.mutable_player_id();
			*pIdent = NFINetModule::NFToPB(self);

			NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
			pDataString->set_property_name( strPropertyName );
			pDataString->set_data( newVar.GetString() );

			m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_PROPERTY_STRING, xPropertyString);
		}
		break;

	case TDATA_OBJECT:
		{
			NFMsg::ObjectPropertyObject xPropertyObject;
			NFMsg::Ident* pIdent = xPropertyObject.mutable_player_id();
			*pIdent = NFINetModule::NFToPB(self);

			NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
			pDataObject->set_property_name( strPropertyName );
			*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

			m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_PROPERTY_OBJECT, xPropertyObject);

		}
		break;

	default:
		break;
	}


	return 0;
}

int NFCWorldGuildEctypeModule::OnGuildObjectRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	const std::string& strRecordName = xEventData.strRecordName;
    int nOpType = xEventData.nOpType;
    int nRow = xEventData.nRow;
    int nCol = xEventData.nCol;

    if ( "Guild" != m_pKernelModule->GetPropertyString( self, "ClassName" ))
    {
        return 0;
    }

    NFCDataList valueBroadCaseList;
    NFCDataList valueBroadCaseGameList;
	const int nServerID = m_pKernelModule->GetPropertyInt(self, NFrame::Guild::EctypServer());
	if (nServerID <= 0)
	{
		return 1;
	}

	valueBroadCaseList << self;
	valueBroadCaseGameList << nServerID;

    if (valueBroadCaseList.GetCount() <= 0 || valueBroadCaseList.GetCount() != valueBroadCaseGameList.GetCount())
    {
        return 0;
    }

    switch ( nOpType )
    {
    case NFIRecord::RecordOptype::Add:
        {
            NFMsg::ObjectRecordAddRow xAddRecordRow;
            NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
            *pIdent = NFINetModule::NFToPB(self);

            xAddRecordRow.set_record_name( strRecordName );

            NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
            pAddRowData->set_row(nRow);

            //add row 需要完整的row
            // FIXME:RECORD
			NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
			if (nullptr == pRecord)
			{
				break;
			}

			NFCDataList varRowData;
			if (!pRecord->QueryRow(nRow, varRowData))
			{
				break;
			}

			for ( int i = 0; i < varRowData.GetCount(); i++ )
            {
                switch ( varRowData.Type( i ) )
                {
                case TDATA_INT:
                    {
                        //添加的时候数据要全
                        int nValue = varRowData.Int( i );
                        if ( 0 != nValue )
                        {
                            NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );
                            pAddData->set_data( nValue );
                        }
                    }
                    break;
                case TDATA_FLOAT:
                    {
                        float fValue = varRowData.Float( i );
                        if ( fValue > 0.001f  || fValue < -0.001f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );
                            pAddData->set_data( fValue );
                        }
                    }
                    break;
                case TDATA_STRING:
                    {
                        const std::string& str = varRowData.String( i );
                        if (!str.empty())
                        {
                            NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );
                            pAddData->set_data( str );
                        }
                    }
                    break;
                case TDATA_OBJECT:
                    {
                        NFGUID identValue = varRowData.Object( i );
                        if (!identValue.IsNull())
                        {
                            NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );

                            *pAddData->mutable_data() = NFINetModule::NFToPB(identValue);
                        }
                    }
                    break;
                default:
                    break;
                }
            }

            m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_ADD_ROW, xAddRecordRow);
        }
        break;
    case NFIRecord::RecordOptype::Del:
        {
            NFMsg::ObjectRecordRemove xReoveRecordRow;

            NFMsg::Ident* pIdent = xReoveRecordRow.mutable_player_id();
            *pIdent = NFINetModule::NFToPB(self);

            xReoveRecordRow.set_record_name( strRecordName );
            xReoveRecordRow.add_remove_row( nRow );

            m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_REMOVE_ROW, xReoveRecordRow);
        }
        break;
    case NFIRecord::RecordOptype::Swap:
        {
            //其实是2个row交换
            NFMsg::ObjectRecordSwap xSwapRecord;
            *xSwapRecord.mutable_player_id() = NFINetModule::NFToPB(self);

            xSwapRecord.set_origin_record_name( strRecordName );
            xSwapRecord.set_target_record_name( strRecordName ); // 暂时没用
            xSwapRecord.set_row_origin( nRow );
            xSwapRecord.set_row_target( nCol );

            m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_SWAP_ROW, xSwapRecord);
        }
        break;
    case NFIRecord::RecordOptype::UpData:
        {
            //装备的过程中是不能更新的
            const int nCol = xEventData.nCol;
            switch ( oldVar.GetType() )
            {
            case TDATA_INT:
                {
                    NFMsg::ObjectRecordInt xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    int nData = newVar.GetInt();
                    recordProperty->set_data( nData );

                    m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_RECORD_INT, xRecordChanged);

                }
                break;

            case TDATA_FLOAT:
                {
                    NFMsg::ObjectRecordFloat xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.GetFloat() );

                    m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_RECORD_DOUBLE, xRecordChanged);

                }
                break;
            case TDATA_STRING:
                {
                    NFMsg::ObjectRecordString xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.GetString() );

                    m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_RECORD_STRING, xRecordChanged);
                }
                break;
            case TDATA_OBJECT:
                {
                    NFMsg::ObjectRecordObject xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    *recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

                    m_pWorldNet_ServerModule->SendMsgToGame(valueBroadCaseList, valueBroadCaseGameList, NFMsg::EGMI_ACK_CLONE_RECORD_OBJECT, xRecordChanged);
                }
                break;

            default:
                return 0;
                break;
            }
        }
        break;
    case NFIRecord::RecordOptype::Create:
        return 0;
        break;
    case NFIRecord::RecordOptype::Cleared:
        {
        }
        break;
    default:
        break;
    }

    return 0;
}

