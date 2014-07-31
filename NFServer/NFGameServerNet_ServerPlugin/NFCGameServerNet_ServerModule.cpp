// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCGameServerNet_ServerModule.h"
#include "NFComm\NFPluginModule\NFILogicModule.h"

bool NFCGameServerNet_ServerModule::Init()
{
    mstrConfigIdent = "GameServer";

    return true;
}

bool NFCGameServerNet_ServerModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pSceneProcessModule = dynamic_cast<NFISceneProcessModule*>(pPluginManager->FindModule("NFCSceneProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);

    m_pKernelModule->ResgisterCommonClassEvent( this, &NFCGameServerNet_ServerModule::OnClassCommonEvent );
    m_pKernelModule->ResgisterCommonPropertyEvent( this, &NFCGameServerNet_ServerModule::OnPropertyCommonEvent );
    m_pKernelModule->ResgisterCommonRecordEvent( this, &NFCGameServerNet_ServerModule::OnRecordCommonEvent );

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCGameServerNet_ServerModule::OnObjectClassEvent );

    const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
    const std::string& strServerIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "ServerIP");
    const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
    const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
    const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
    const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
    const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

    m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCGameServerNet_ServerModule::OnRecivePack, &NFCGameServerNet_ServerModule::OnSocketEvent);
    int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
    if (nRet <= 0)
    {
        assert(0);
    }


    return true;
}

bool NFCGameServerNet_ServerModule::Shut()
{

    return true;
}

bool NFCGameServerNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

int NFCGameServerNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
    // TOCHECK
    //NFIActorMessage xActorMsg;

    //xActorMsg.eType = NFIActorMessage::EACTOR_NET_MSG;
    //xActorMsg.nSubMsgID = msg.GetMsgHead()->GetMsgID();
    //xActorMsg.data.assign(msg.GetData(), msg.GetDataLen());

    ////�����û�������Ϣ�õ�Ŀ��actor��ַ,���Լ�����ô������Ż�
    //pPluginManager->GetFramework().Send(xActorMsg, pPluginManager->GetAddress(), pPluginManager->GetAddress());
    //����actor�����
    switch (msg.GetMsgHead()->GetMsgID())
    {
    case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REFRESH:
        OnRefreshProxyServerInfoProcess(msg);
        break;

    case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED:
        OnProxyServerRegisteredProcess(msg);
        break;

    case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_UNREGISTERED:
        OnProxyServerUnRegisteredProcess(msg);
        break;
        //////////////////////////////////////////////////////////////////////////
    case NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME:
        OnClienEnterGameProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GAME:
        OnClienLeaveGameProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST:
        OnReqiureRoleListProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE:
        OnCreateRoleGameProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_DELETE_ROLE:
        OnDeleteRoleGameProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_RECOVER_ROLE:
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_SWAP_SCENE:
        OnClienSwapSceneProcess(msg);
         break;
    case NFMsg::EGameMsgID::EGMI_REQ_COMMAND:
        OnClienGMProcess(msg);
        break;
    default:
        break;
    }

    return true;
}

int NFCGameServerNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }

    return true;
}

void NFCGameServerNet_ServerModule::OnClientDisconnect( const int nAddress )
{
    //ֻ���������ض���
    int nServerID = 0;
    ServerData* pServerData =  mProxyMap.First();
    while (pServerData)
    {
        if (nAddress == pServerData->nFD)
        {
            nServerID = pServerData->pData->server_id();
            break;
        }

        pServerData = mProxyMap.Next();
    }

    if (nServerID != 0)
    {
        ServerData* pRmServerData = mProxyMap.RemoveElement(nServerID);
        if (pRmServerData)
        {
            delete pRmServerData;
            pRmServerData = NULL;
        }
    }
}

void NFCGameServerNet_ServerModule::OnClientConnected( const int nAddress )
{

}

void NFCGameServerNet_ServerModule::OnClienEnterGameProcess( const NFIPacket& msg )
 {
    //�ڽ�����Ϸ֮ǰnPlayerIDΪ�������ص�FD
    int64_t nPlayerID = 0;	
    NFMsg::ReqEnterGameServer xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    //server_id
    //gate_id

    if (mRoleState.GetElement(xMsg.name()))
    {
        //��ɫ�Ѿ��ڴ�����
        return;
    }

    mRoleState.AddElement(xMsg.name(), new int(nPlayerID));

    //////////////////////////////////////////////////////////////////////////
    //��ȡ����
    //////////////////////////////////////////////////////////////////////////
    

    int nGateID = -1;
    ServerData* pServerData =  mProxyMap.First();
    while (pServerData)
    {
        if (msg.GetFd() == pServerData->nFD)
        {
            nGateID = pServerData->pData->server_id();
            break;
        }

        pServerData = mProxyMap.Next();
    }

    if (nGateID <= 0)
    {
        //�Ƿ�gate
        return;
    }

    //id��fd,gateid��
    NFIDENTID ident = NFIDENTID::CreateGUID();

    BaseData* pDataBase = new BaseData();
    mRoleBaseData.AddElement(ident, pDataBase);
    mRoleFDData.AddElement(nPlayerID, new NFIDENTID(ident));

    pDataBase->nGateID = nGateID;
    pDataBase->nFD = nPlayerID;

    int nSceneID = 1;
    NFCDataList var;
    var << "Name" << xMsg.name();
    var << "GateID" << nGateID;
    var << "FD" << (int)nPlayerID;

    NFIObject* pObject = m_pKernelModule->CreateObject(ident, nSceneID, 0, "Player", "",  var);
    if ( NULL == pObject )
    {
        //�ڴ�й©
        //mRoleBaseData
        //mRoleFDData
        return;
    }

    pObject->SetPropertyInt("LoadPropertyFinish", 1);
    
    m_pEventProcessModule->DoEvent(pObject->Self(), "Player", CLASS_OBJECT_EVENT::COE_CREATE_FINISH, NFCDataList() );

    NFCDataList varEntry;
    varEntry << pObject->Self();
    varEntry << 0;
    varEntry << nSceneID;
    varEntry << -1;
    m_pEventProcessModule->DoEvent( pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry );
}

void NFCGameServerNet_ServerModule::OnClienLeaveGameProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;	
    NFMsg::ReqLeaveGameServer xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    if (0 == nPlayerID)
    {
        return;
    }

    NFIDENTID* pIdent = mRoleFDData.RemoveElement(nPlayerID);
    if (pIdent)
    {
        m_pKernelModule->DestroyObject(*pIdent);
    }

}

int NFCGameServerNet_ServerModule::OnPropertyEnter( const NFIDENTID& self, const NFIDataList& argVar )
{
    if ( argVar.GetCount() <= 0 || self.IsNull())
    {
        return 0;
    }

    NFMsg::MultiObjectPropertyList xPublicMsg;
    NFMsg::MultiObjectPropertyList xPrivateMsg;

    //��Ϊ�Լ�������
    //1.public���͸�������
    //2.����Լ����б��У��ٴη���private����
    NFIObject* pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();
        NFMsg::ObjectPropertyList* pPrivateData = xPrivateMsg.add_multi_player_property();
        pPublicData->set_player_id(self.nData64);
        pPrivateData->set_player_id(self.nData64);

        NFIPropertyManager* pPropertyManager = pObject->GetPropertyManager();
        NFIProperty* pPropertyInfo = pPropertyManager->First();
        while ( pPropertyInfo )
        {
            if ( pPropertyInfo->Changed() )
            {
                switch ( pPropertyInfo->GetType())
                {
                case TDATA_INT:
                    {
                        if ( pPropertyInfo->GetPublic() )
                        {
                            NFMsg::PropertyInt* pDataInt = pPublicData->add_property_int_list();
                            pDataInt->set_property_name( pPropertyInfo->GetKey() );
                            pDataInt->set_data( pPropertyInfo->GetInt() );
                        }

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
                        if ( pPropertyInfo->GetPublic() )
                        {
                            NFMsg::PropertyFloat* pDataFloat = pPublicData->add_property_float_list();
                            pDataFloat->set_property_name( pPropertyInfo->GetKey() );
                            pDataFloat->set_data( pPropertyInfo->GetInt() );
                        }

                        if ( pPropertyInfo->GetPrivate() )
                        {
                            NFMsg::PropertyFloat* pDataFloat = pPrivateData->add_property_float_list();
                            pDataFloat->set_property_name( pPropertyInfo->GetKey() );
                            pDataFloat->set_data( pPropertyInfo->GetFloat() );
                        }
                    }
                    break;

                case TDATA_DOUBLE:
                    {
                        if ( pPropertyInfo->GetPublic() )
                        {
                            NFMsg::PropertyFloat* pDataFloat = pPublicData->add_property_float_list();
                            pDataFloat->set_property_name( pPropertyInfo->GetKey() );
                            pDataFloat->set_data( pPropertyInfo->GetDouble() );
                        }

                        if ( pPropertyInfo->GetPrivate() )
                        {
                            NFMsg::PropertyFloat* pDataFloat = pPrivateData->add_property_float_list();
                            pDataFloat->set_property_name( pPropertyInfo->GetKey() );
                            pDataFloat->set_data( pPropertyInfo->GetDouble() );
                        }
                    }
                    break;

                case TDATA_STRING:
                    {
                        if ( pPropertyInfo->GetPublic() )
                        {
                            NFMsg::PropertyString* pDataString = pPublicData->add_property_string_list();
                            pDataString->set_property_name( pPropertyInfo->GetKey() );
                            pDataString->set_data( pPropertyInfo->GetString() );
                        }

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
                        if ( pPropertyInfo->GetPublic() )
                        {
                            NFMsg::PropertyObject* pDataObject = pPublicData->add_property_object_list();
                            pDataObject->set_property_name( pPropertyInfo->GetKey() );
                            pDataObject->set_data( pPropertyInfo->GetObject().nData64 );
                        }

                        if ( pPropertyInfo->GetPrivate() )
                        {
                            NFMsg::PropertyObject* pDataObject = pPrivateData->add_property_object_list();
                            pDataObject->set_property_name( pPropertyInfo->GetKey() );
                            pDataObject->set_data( pPropertyInfo->GetObject().nData64 );
                        }
                    }
                    break;

                default:
                    break;
                }
            }

            pPropertyInfo = pPropertyManager->Next();
        }

        for ( int i = 0; i < argVar.GetCount(); i++ )
        {
            NFIDENTID identOther = argVar.Object( i );
            if ( self == identOther )
            {
                //�ҵ����������ص�FD
                BaseData* pData = mRoleBaseData.GetElement(identOther);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, xPrivateMsg, pProxyData->nFD, pData->nFD);
                    }
                }
            }
            else
            {
                BaseData* pData = mRoleBaseData.GetElement(identOther);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, xPublicMsg, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
    }

    return 0;
}

bool OnRecordEnterPack(NFIRecord* pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase)
{
    if (!pRecord || !pObjectRecordBase)
    {
        return false;
    }

    for ( int i = 0; i < pRecord->GetRows(); i ++ )
    {
        if ( pRecord->IsUsed( i ) )
        {
            //����public����private��Ҫ���ϣ���Ȼpublic�㲥���ǲ���private�͹㲥������
            NFMsg::RecordAddRowStruct* pAddRowStruct = pObjectRecordBase->add_row_struct();
            pAddRowStruct->set_row( i );

            for ( int j = 0; j < pRecord->GetCols(); j++ )
            {
                //�����0�Ͳ������ˣ���Ϊ�ͻ���Ĭ����0
                NFCDataList valueList;
                TDATA_TYPE eType = pRecord->GetColType( j );
                switch ( eType )
                {
                case TDATA_TYPE::TDATA_INT:
                    {
                        int nValue = pRecord->GetInt( i, j );
                        //if ( 0 != nValue )
                        {
                            NFMsg::RecordInt* pRecordInt = pAddRowStruct->add_record_int_list();
                            pRecordInt->set_row( i );
                            pRecordInt->set_col( j );
                            pRecordInt->set_data( nValue );
                        }
                    }
                    break;
                case TDATA_TYPE::TDATA_DOUBLE:
                    {
                        double dwValue = pRecord->GetDouble( i, j );
                        //if ( dwValue < -0.01f || dwValue > 0.01f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowStruct->add_record_float_list();
                            pAddData->set_col( j );
                            pAddData->set_data( dwValue );
                        }
                    }
                    break;
                case TDATA_TYPE::TDATA_FLOAT:
                    {
                        float fValue = pRecord->GetFloat( i, j );
                        //if ( fValue < -0.01f || fValue > 0.01f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowStruct->add_record_float_list();
                            pAddData->set_col( j );
                            pAddData->set_data( fValue );
                        }
                    }
                    break;
                case TDATA_TYPE::TDATA_STRING:
                    {
                        const std::string& strData = pRecord->GetString( i, j );
                        //if ( !strData.empty() )
                        {
                            NFMsg::RecordString* pAddData = pAddRowStruct->add_record_string_list();
                            pAddData->set_col( j );
                            pAddData->set_data( strData );
                        }
                    }
                    break;
                case TDATA_TYPE::TDATA_OBJECT:
                    {
                        NFIDENTID ident = pRecord->GetObject( i, j );
                        //if ( !ident.IsNull() )
                        {
                            NFMsg::RecordObject* pAddData = pAddRowStruct->add_record_object_list();
                            pAddData->set_col( j );
                            pAddData->set_data( ident.nData64 );
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    return true;
}

int NFCGameServerNet_ServerModule::OnRecordEnter( const NFIDENTID& self, const NFIDataList& argVar )
{
    if ( argVar.GetCount() <= 0 || self.IsNull() )
    {
        return 0;
    }

    NFMsg::MultiObjectRecordList xPublicMsg;
    NFMsg::MultiObjectRecordList xPrivateMsg;

    NFIObject* pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NFMsg::ObjectRecordList* pPublicData = NULL;
        NFMsg::ObjectRecordList* pPrivateData = NULL;
        
        NFIRecordManager* pRecordManager = pObject->GetRecordManager();
        NFIRecord* pRecord = pRecordManager->First();
        while ( pRecord )
        {
            if (!pRecord->GetPublic() && !pRecord->GetPrivate())
            {
                pRecord = pRecordManager->Next();
                continue;
            }

            NFMsg::ObjectRecordBase* pPrivateRecordBase = NULL;
            NFMsg::ObjectRecordBase* pPublicRecordBase = NULL;
            if (pRecord->GetPublic())
            {
                if (!pPublicData)
                {
                    pPublicData = xPublicMsg.add_multi_player_record();
                    pPublicData->set_player_id(self.nData64);
                }
                pPublicRecordBase = pPublicData->add_record_list();
                pPublicRecordBase->set_record_name(pRecord->GetName());

                OnRecordEnterPack(pRecord, pPublicRecordBase);
            }

            if (pRecord->GetPrivate())
            {
                if (!pPrivateData)
                {
                    pPrivateData = xPrivateMsg.add_multi_player_record();
                    pPrivateData->set_player_id(self.nData64);
                }
                pPrivateRecordBase = pPrivateData->add_record_list();
                pPrivateRecordBase->set_record_name(pRecord->GetName());

                OnRecordEnterPack(pRecord, pPrivateRecordBase);
            }

            pRecord = pRecordManager->Next();
        }


        for ( int i = 0; i < argVar.GetCount(); i++ )
        {
            NFIDENTID identOther = argVar.Object( i );
            if ( self == identOther )
            {
                if (xPrivateMsg.multi_player_record_size() > 0)
                {
                    //������Լ�����ôֻ�㲥˽�еģ��������˽�е�û������
                    //�ҵ����������ص�FD
                    BaseData* pData = mRoleBaseData.GetElement(identOther);
                    if (pData)
                    {
                        ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                        if (pProxyData)
                        {
                            SendMsgPB(NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, xPrivateMsg, pProxyData->nFD, pData->nFD);

                        }
                    }
                }
            }
            else
            {
                if (xPublicMsg.multi_player_record_size() > 0)
                {
                    //��������Լ�����ô�㲥���еģ�����������е�û������
                    BaseData* pData = mRoleBaseData.GetElement(identOther);
                    if (pData)
                    {
                        ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                        if (pProxyData)
                        {
                            SendMsgPB(NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, xPublicMsg, pProxyData->nFD, pData->nFD);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int NFCGameServerNet_ServerModule::OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar )
{
    if ( self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
    for ( int i = 0; i < argVar.GetCount(); i++ )
    {
        NFIDENTID identOld = argVar.Object( i );
        //�ų��ն���
        if (identOld.IsNull())
        {
            continue;
        }

        NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
        pEntryInfo->set_object_guid( identOld.nData64 );
        pEntryInfo->set_x( m_pKernelModule->GetPropertyFloat( identOld, "X" ) );
        pEntryInfo->set_y( m_pKernelModule->GetPropertyFloat( identOld, "Y" ) );
        pEntryInfo->set_z( m_pKernelModule->GetPropertyFloat( identOld, "Z" ) );
        pEntryInfo->set_career_type( m_pKernelModule->GetPropertyInt( identOld, "Job" ) );
        pEntryInfo->set_player_state( m_pKernelModule->GetPropertyInt( identOld, "State" ) );
        pEntryInfo->set_config_id( m_pKernelModule->GetPropertyString( identOld, "ConfigID" ) );
        pEntryInfo->set_scene_id( m_pKernelModule->GetPropertyInt( identOld, "SceneID" ) );
        pEntryInfo->set_class_id( m_pKernelModule->GetPropertyString( identOld, "ClassName" ) );

    }

    if (xPlayerEntryInfoList.object_list_size() <= 0)
    {
        return 0;
    }

    for (int i = 0; i < self.GetCount(); i++)
    {
        NFIDENTID ident = self.Object(i);
        if (ident.IsNull())
        {
            continue;
        }

        //�����ڲ�ͬ��������,�õ��������ڵ�����FD
        BaseData* pData = mRoleBaseData.GetElement(ident);
        if (pData)
        {
            ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
            if (pProxyData)
            {
                SendMsgPB(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerEntryInfoList, pProxyData->nFD, pData->nFD);
            }
        }
    }        

    return 1;
}

int NFCGameServerNet_ServerModule::OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar )
{
    if ( self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
    for ( int i = 0; i < argVar.GetCount(); i++ )
    {
        NFIDENTID identOld = argVar.Object( i );
        //�ų��ն���
        if (identOld.IsNull())
        {
            continue;
        }

        xPlayerLeaveInfoList.add_object_list(argVar.Object(i).nData64);
    }

    for (int i = 0; i < self.GetCount(); i++)
    {
        NFIDENTID ident = self.Object(i);
        if (ident.IsNull())
        {
            continue;
        }
        //�����ڲ�ͬ��������,�õ��������ڵ�����FD
        BaseData* pData = mRoleBaseData.GetElement(ident);
        if (pData)
        {
            ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
            if (pProxyData)
            {
                SendMsgPB(NFMsg::EGMI_ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, pProxyData->nFD, pData->nFD);
            }
        }
    }

    return 1;
}

int NFCGameServerNet_ServerModule::OnPropertyCommonEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{
    if ( oldVar.GetCount() <= 0 )
    {
        return 0;
    }

    if ( "Player" == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
    {
        if ( "GroupID" == strPropertyName )
        {
            //�Լ�����Ҫ֪���Լ���������Ա仯��,���Ǳ��˾Ͳ���Ҫ֪����
            OnGroupEvent( self, strPropertyName, oldVar, newVar, argVar );
        }

        if ( "SceneID" == strPropertyName )
        {
            //�Լ�����Ҫ֪���Լ���������Ա仯��,���Ǳ��˾Ͳ���Ҫ֪����
            OnContainerEvent( self, strPropertyName, oldVar, newVar, argVar );
        }

        if ( m_pKernelModule->GetPropertyInt( self, "LoadPropertyFinish" ) <= 0 )
        {
            return 0;
        }
    }

    NFCDataList valueBroadCaseList;
    int nCount = argVar.GetCount() ;
    if ( nCount <= 0 )
    {
        nCount = GetBroadCastObject( self, strPropertyName, false, valueBroadCaseList );
    }
    else
    {
        //����Ĳ�����Ҫ�㲥�Ķ����б�
        valueBroadCaseList = argVar;
    }

    if ( valueBroadCaseList.GetCount() <= 0 )
    {
        return 0;
    }

    switch ( oldVar.Type( 0 ) )
    {
    case TDATA_INT:
        {
            NFMsg::ObjectPropertyInt xPropertyInt;
            xPropertyInt.set_player_id( self.nData64 );
            NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
            pDataInt->set_property_name( strPropertyName );
            pDataInt->set_data( newVar.Int( 0 ) );
            
            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOld);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_PROPERTY_INT, xPropertyInt, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;

    case TDATA_FLOAT:
        {
            NFMsg::ObjectPropertyFloat xPropertyFloat;
            xPropertyFloat.set_player_id( self.nData64 );
            NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
            pDataFloat->set_property_name( strPropertyName );
            pDataFloat->set_data( newVar.Float( 0 ) );
            
            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOld);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_PROPERTY_FLOAT, xPropertyFloat, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;

    case TDATA_DOUBLE:
        {
            NFMsg::ObjectPropertyFloat xPropertyDouble;
            xPropertyDouble.set_player_id( self.nData64 );
            NFMsg::PropertyFloat* pDataFloat = xPropertyDouble.add_property_list();
            pDataFloat->set_property_name( strPropertyName );
            pDataFloat->set_data( newVar.Double( 0 ) );
            
            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOld);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_PROPERTY_DOUBLE, xPropertyDouble, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;

    case TDATA_STRING:
        {
            NFMsg::ObjectPropertyString xPropertyString;
            xPropertyString.set_player_id( self.nData64 );
            NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
            pDataString->set_property_name( strPropertyName );
            pDataString->set_data( newVar.String( 0 ) );
            
            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOld);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_PROPERTY_STRING, xPropertyString, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;

    case TDATA_OBJECT:
        {
            NFMsg::ObjectPropertyObject xPropertyObject;
            xPropertyObject.set_player_id( self.nData64 );

            NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
            pDataObject->set_property_name( strPropertyName );
            pDataObject->set_data( newVar.Object( 0 ).nData64 );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOld);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_PROPERTY_OBJECT, xPropertyObject, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;

    default:
        break;
    }

    
    return 0;
}

int NFCGameServerNet_ServerModule::OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{
    int nObjectContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
    int nObjectGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

    if ( nObjectGroupID < 0 )
    {
        //����
        return 0;
    }
    if ( "Player" == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
    {
        if (m_pKernelModule->GetPropertyInt(self, "LoadPropertyFinish") <= 0)
        {
            return 0;
        }
    }

    NFCDataList valueBroadCaseList;
    GetBroadCastObject( self, strRecordName, true, valueBroadCaseList );

    switch ( nOpType )
    {
    case NFIRecord::RecordOptype::Add:
        {
            NFMsg::ObjectRecordAddRow xAddRecordRow;

            xAddRecordRow.set_player_id( self.nData64 );
            xAddRecordRow.set_record_name( strRecordName );

            NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
            pAddRowData->set_row(nRow);

            //add row ��Ҫ������row
            for ( int i = 0; i < newVar.GetCount(); i++ )
            {
                switch ( newVar.Type( i ) )
                {
                case TDATA_INT:
                    {
                        //��ӵ�ʱ������Ҫȫs
                        int nValue = newVar.Int( i );
                        //if ( 0 != nValue )
                        {
                            NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
                            pAddData->set_col( i );
                            pAddData->set_data( nValue );
                        }                        
                    }
                    break;
                case TDATA_FLOAT:
                    {
                        float fValue = newVar.Float( i );
                        //if ( fValue > 0.001f  || fValue < -0.001f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
                            pAddData->set_col( i );
                            pAddData->set_data( fValue );
                        }                        
                    }
                    break;
                case TDATA_DOUBLE:
                    {
                        float fValue = newVar.Double( i );
                        //if ( fValue > 0.001f  || fValue < -0.001f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
                            pAddData->set_col( i );
                            pAddData->set_data( fValue );
                        }                        
                    }
                    break;
                case TDATA_STRING:
                    {
                        const std::string& str = newVar.String( i );
                        //if (!str.empty())
                        {
                            NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
                            pAddData->set_col( i );
                            pAddData->set_data( str );
                        }                        
                    }
                    break;
                case TDATA_OBJECT:
                    {
                        NFIDENTID identValue = newVar.Object( i );
                        //if (!identValue.IsNull())
                        {
                            NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
                            pAddData->set_col( i );
                            pAddData->set_data( identValue.nData64 );
                        }
                    }
                    break;
                default:
                    break;
                }
            }

  
            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOther = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOther);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_ADD_ROW, xAddRecordRow, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;
    case NFIRecord::RecordOptype::Del:
        {
            NFMsg::ObjectRecordRemove xReoveRecordRow;
            xReoveRecordRow.set_player_id( self.nData64 );
            xReoveRecordRow.set_record_name( strRecordName );
            xReoveRecordRow.add_remove_row( nRow );
            
            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOther = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOther);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_REMOVE_ROW, xReoveRecordRow, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;
    case NFIRecord::RecordOptype::Changed:
        {
            //��ʵ��2��row����
            NFMsg::ObjectRecordSwap xSwapRecord;
            xSwapRecord.set_player_id( self.nData64 );
            xSwapRecord.set_origin_record_name( strRecordName );
            xSwapRecord.set_target_record_name( strRecordName ); // ��ʱû��
            xSwapRecord.set_row_origin( nRow );
            xSwapRecord.set_row_target( nCol );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOther = valueBroadCaseList.Object( i );
                BaseData* pData = mRoleBaseData.GetElement(identOther);
                if (pData)
                {
                    ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                    if (pProxyData)
                    {
                        SendMsgPB(NFMsg::EGMI_ACK_SWAP_ROW, xSwapRecord, pProxyData->nFD, pData->nFD);
                    }
                }
            }
        }
        break;
    case NFIRecord::RecordOptype::UpData:
        {
            switch ( oldVar.Type( 0 ) )
            {
            case TDATA_INT:
                {
                    NFMsg::ObjectRecordInt xRecordChanged;
                    xRecordChanged.set_player_id( self.nData64 );
                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    int nData = newVar.Int( 0 );
                    recordProperty->set_data( nData );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        BaseData* pData = mRoleBaseData.GetElement(identOther);
                        if (pData)
                        {
                            ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                            if (pProxyData)
                            {
                                SendMsgPB(NFMsg::EGMI_ACK_RECORD_INT, xRecordChanged, pProxyData->nFD, pData->nFD);
                            }
                        }
                    }
                }
                break;

            case TDATA_FLOAT:
                {
                    NFMsg::ObjectRecordFloat xRecordChanged;
                    xRecordChanged.set_player_id( self.nData64 );
                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.Float( 0 ) );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        BaseData* pData = mRoleBaseData.GetElement(identOther);
                        if (pData)
                        {
                            ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                            if (pProxyData)
                            {
                                SendMsgPB(NFMsg::EGMI_ACK_RECORD_FLOAT, xRecordChanged, pProxyData->nFD, pData->nFD);
                            }
                        }
                    }
                }
            case TDATA_DOUBLE:
                {
                    NFMsg::ObjectRecordFloat xRecordChanged;
                    xRecordChanged.set_player_id( self.nData64 );
                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.Double( 0 ) );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        BaseData* pData = mRoleBaseData.GetElement(identOther);
                        if (pData)
                        {
                            ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                            if (pProxyData)
                            {
                                SendMsgPB(NFMsg::EGMI_ACK_RECORD_DOUBLE, xRecordChanged, pProxyData->nFD, pData->nFD);
                            }
                        }
                    }
                }
                break;
            case TDATA_STRING:
                {
                    NFMsg::ObjectRecordString xRecordChanged;
                    xRecordChanged.set_player_id( self.nData64 );
                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.String( 0 ) );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        BaseData* pData = mRoleBaseData.GetElement(identOther);
                        if (pData)
                        {
                            ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                            if (pProxyData)
                            {
                                SendMsgPB(NFMsg::EGMI_ACK_RECORD_STRING, xRecordChanged, pProxyData->nFD, pData->nFD);
                            }
                        }
                    }
                }
                break;
            case TDATA_OBJECT:
                {
                    NFMsg::ObjectRecordObject xRecordChanged;
                    xRecordChanged.set_player_id( self.nData64 );
                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.Object( 0 ).nData64 );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        BaseData* pData = mRoleBaseData.GetElement(identOther);
                        if (pData)
                        {
                            ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
                            if (pProxyData)
                            {
                                SendMsgPB(NFMsg::EGMI_ACK_RECORD_OBJECT, xRecordChanged, pProxyData->nFD, pData->nFD);
                            }
                        }
                    }
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
//             NFMsg::ObjectRecordObject xRecordChanged;
//             xRecordChanged.set_player_id( self.nData64 );
//             xRecordChanged.set_record_name( strRecordName );
// 
//             for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
//             {
//                 NFIDENTID identOther = valueBroadCaseList.Object( i );
//                 SendMsgPB(NFMsg::EGMI_ACK_RECORD_CLEAR, xRecordChanged, 0);
//             }
        }
        break;
    default:
        break;
    }

    return 0;
}

int NFCGameServerNet_ServerModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    ////////////1:�㲥���Ѿ����ڵ���//////////////////////////////////////////////////////////////
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {
        //ɾ�����߱�־
        const std::string& strRole = m_pKernelModule->GetPropertyString( self, "Name" );
        int* pInt = mRoleState.RemoveElement(strRole);
        if (pInt)
        {
            delete pInt;
        }
        //////////////////////////////////////////////////////////////////////////

        int nObjectContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
        int nObjectGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

        if ( nObjectGroupID < 0 )
        {
            //����
            return 0;
        }

        NFCDataList valueAllObjectList;
        NFCDataList valueBroadCaseList;
        NFCDataList valueBroadListNoSelf;
        m_pKernelModule->GetGroupObjectList( nObjectContainerID, nObjectGroupID, valueAllObjectList );

        for ( int i = 0; i < valueAllObjectList.GetCount(); i++ )
        {
            NFIDENTID identBC = valueAllObjectList.Object( i );
            const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
            if ( "Player" == strClassName )
            {
                valueBroadCaseList.Add( identBC );
                if (identBC != self)
                {
                    valueBroadListNoSelf.Add( identBC );
                }
            }
        }

        //����Ǹ����Ĺ֣�����Ҫ���ͣ���Ϊ�����뿪������ʱ��һ����һ����Ϣ����
        OnObjectListLeave( valueBroadListNoSelf, NFCDataList() << self );
    }

    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
        //id��fd,gateid��
//         BaseData* pDataBase = new BaseData();
//         mRoleBaseData.AddElement(self, pDataBase);
// 
//         pDataBase->nGateID = xMsg.gate_id();
//         pDataBase->nFD = nPlayerID;
        //�ظ��ͻ��˽�ɫ������Ϸ����ɹ���
//         NFMsg::AckEventResult xMsg;
//         xMsg.set_event_code(NFMsg::EGEC_ACCOUNTPWD_INVALID);
//         xMsg.set_event_arg(self.nData64);
// 
//         SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ENTER_GAME, xMsg, unAddress);
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
    {
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
    {
        //�Լ��㲥���Լ��͹���
        if ( strClassName == "Player" )
        {
            OnObjectListEnter( NFCDataList() << self, NFCDataList() << self );

            OnPropertyEnter( self, NFCDataList() << self );
            OnRecordEnter( self, NFCDataList() << self );
        }
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
    {

    }
    return 0;
}

int NFCGameServerNet_ServerModule::OnGroupEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{
    //���������仯��ֻ���ܴ�A������0���л���B������0��
    //��Ҫע�����------------�κβ�ı��ʱ�򣬴������ʵ��δ����㣬��ˣ���ı��ʱ���ȡ������б�Ŀ����ǲ������Լ���
    int nSceneID = m_pKernelModule->GetPropertyInt( self, "SceneID" );

    //�㲥�������Լ���ȥ(�㽵����Ծ��)
    int nOldGroupID = oldVar.NumberVal<int>( 0 );
    if ( nOldGroupID > 0 )
    {
        NFCDataList valueAllOldObjectList;
        NFCDataList valueAllOldPlayerList;
        m_pKernelModule->GetGroupObjectList( nSceneID, nOldGroupID, valueAllOldObjectList );
        if ( valueAllOldObjectList.GetCount() > 0 )
        {
            //�Լ�ֻ��Ҫ�㲥�������
            for ( int i = 0; i < valueAllOldObjectList.GetCount(); i++ )
            {
                NFIDENTID identBC = valueAllOldObjectList.Object( i );

                if(valueAllOldObjectList.Object(i) == self)
                {
                    valueAllOldObjectList.Set(i, NFIDENTID(0));
                }

                const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
                if ( "Player" == strClassName )
                {
                    valueAllOldPlayerList.Add(identBC );
                }
            }

            OnObjectListLeave( valueAllOldPlayerList, NFCDataList() << self );

            //�ϵ�ȫ��Ҫ�㲥ɾ��
            OnObjectListLeave( NFCDataList() << self, valueAllOldObjectList );
        }

        m_pEventProcessModule->DoEvent(self, NFED_ON_CLIENT_LEAVE_SCENE, NFCDataList() << nOldGroupID);

    }

    //�ٹ㲥�������Լ�����(��������Ծ��)
    int nNewGroupID = newVar.NumberVal<int>( 0 );
    if ( nNewGroupID > 0 )
    {
        //������Ҫ���Լ��ӹ㲥���ų�
        //////////////////////////////////////////////////////////////////////////
        NFCDataList valueNewObjectList;
        NFCDataList valueNewObjectListNoSelf;
        NFCDataList valueBroadCaseList;
        NFCDataList valueBroadCaseListNoSelf;
        m_pKernelModule->GetGroupObjectList( nSceneID, nNewGroupID, valueNewObjectList );
        for ( int i = 0; i < valueNewObjectList.GetCount(); i++ )
        {
            NFIDENTID identBC = valueNewObjectList.Object( i );
            const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
            if ( "Player" == strClassName )
            {
                valueBroadCaseList.Add( identBC );
                if (identBC != self)
                {
                    valueBroadCaseListNoSelf.Add( identBC );
                }
            }

            if (identBC != self)
            {
                valueNewObjectListNoSelf.Add( identBC );
            }
        }

        //�㲥������,�Լ�����(���ﱾ��Ӧ�ù㲥���Լ�)
        if ( valueBroadCaseListNoSelf.GetCount() > 0 )
        {
            OnObjectListEnter( valueBroadCaseListNoSelf, NFCDataList() << self );
        }

        //�㲥���Լ�,���еı��˳���
        if ( valueNewObjectListNoSelf.GetCount() > 0 )
        {
            OnObjectListEnter( NFCDataList() << self, valueNewObjectListNoSelf );
        }

        for ( int i = 0; i < valueNewObjectListNoSelf.GetCount(); i++ )
        {
            //��ʱ�����ٹ㲥�Լ������Ը��Լ�
            //���Ѿ����ڵ��˵����Թ㲥����������
            NFIDENTID identOld = valueNewObjectListNoSelf.Object( i );
            OnPropertyEnter( identOld, NFCDataList() << self );
            //���Ѿ����ڵ��˵ı�㲥����������
            OnRecordEnter( identOld, NFCDataList() << self );
        }

        //���������˵����Թ㲥���ܱߵ���
        if ( valueBroadCaseListNoSelf.GetCount() > 0 )
        {
            OnPropertyEnter( self, valueBroadCaseListNoSelf );
            OnRecordEnter( self, valueBroadCaseListNoSelf );
        }

    }
    return 0;
}

int NFCGameServerNet_ServerModule::OnContainerEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{
    //���������仯��ֻ���ܴ�A������0���л���B������0��
    //��Ҫע�����------------�κ������ı��ʱ����ұ�����0��
    int nOldSceneID = oldVar.NumberVal<int>( 0 );
    int nNowSceneID = newVar.NumberVal<int>( 0 );

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Enter Scene:", nNowSceneID);

    //�Լ���ʧ,��Ҳ��ù㲥����Ϊ����ʧ֮ǰ����ص�0�㣬���ѹ㲥�����
    NFCDataList valueOldAllObjectList;
    NFCDataList valueNewAllObjectList;
    NFCDataList valueAllObjectListNoSelf;
    NFCDataList valueBroadCaseList;
    NFCDataList valueBroadListNoSelf;

    m_pKernelModule->GetGroupObjectList( nOldSceneID, 0, valueOldAllObjectList );
    m_pKernelModule->GetGroupObjectList( nNowSceneID, 0, valueNewAllObjectList );

    for ( int i = 0; i < valueOldAllObjectList.GetCount(); i++ )
    {
        NFIDENTID identBC = valueOldAllObjectList.Object( i );
        if (identBC == self)
        {
            valueOldAllObjectList.Set(i, NFIDENTID(0));
        }
    }

    for ( int i = 0; i < valueNewAllObjectList.GetCount(); i++ )
    {
        NFIDENTID identBC = valueNewAllObjectList.Object( i );
        const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
        if ( "Player" == strClassName )
        {
            valueBroadCaseList.Add( identBC );
            if (identBC != self)
            {
                valueBroadListNoSelf.Add( identBC );
            }
        }

        if (identBC != self)
        {
            valueAllObjectListNoSelf.Add( identBC );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    //�ر�ģ���5ţ��Ҳת������
    NFCDataList varMasterPlayerList;
    valueAllObjectListNoSelf.Append(varMasterPlayerList, 0, varMasterPlayerList.GetCount());
    //////////////////////////////////////////////////////////////////////////

    //���Ǿɳ���0���NPC��Ҫ�㲥
    OnObjectListLeave( NFCDataList() << self, valueOldAllObjectList );

    //�㲥�������˳��ֶ���(�������ң�������㲥���Լ�)
    //����㲥�Ķ���0���
    if ( valueBroadCaseList.GetCount() > 0 )
    {
        //��self�㲥��argVar��Щ��
        OnObjectListEnter( valueBroadListNoSelf, NFCDataList() << self);
    }

    //�²��Ȼ��0����0��NPC�㲥���Լ�------------�Լ��㲥���Լ���������㲥����Ϊ����ID�ڿ糡��ʱ�ᾭ���仯

    //��valueAllObjectList�㲥��self
    OnObjectListEnter( NFCDataList() << self, valueAllObjectListNoSelf );

    ////////////////////���Ѿ����ڵ��˵����Թ㲥����������//////////////////////////////////////////////////////
    for ( int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++ )
    {
        NFIDENTID identOld = valueAllObjectListNoSelf.Object( i );
        OnPropertyEnter( identOld, NFCDataList() << self );
        ////////////////////���Ѿ����ڵ��˵ı�㲥����������//////////////////////////////////////////////////////
        OnRecordEnter( identOld, NFCDataList() << self );
    }

    //���������˵����Թ㲥���ܱߵ���()
    if ( valueBroadListNoSelf.GetCount() > 0 )
    {
        OnPropertyEnter( self, valueBroadListNoSelf );
        OnRecordEnter( self, valueBroadListNoSelf );
    }

    return 0;
}

int NFCGameServerNet_ServerModule::GetBroadCastObject( const NFIDENTID& self, const std::string& strPropertyName, const bool bTable, NFIDataList& valueObject )
{
    int nObjectContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
    int nObjectGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

    //��ͨ�����������жϹ㲥����
    std::string strClassName = m_pKernelModule->GetPropertyString( self, "ClassName" );
    NFIRecordManager* pClassRecordManager = m_pLogicClassModule->GetClassRecordManager( strClassName );
    NFIPropertyManager* pClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager( strClassName );

    NFIRecord* pRecord = NULL;
    NFIProperty* pProperty = NULL;
    if ( bTable )
    {
        if ( NULL == pClassRecordManager )
        {
            return -1;
        }

        pRecord = pClassRecordManager->GetElement( strPropertyName );
        if ( NULL == pRecord )
        {
            return -1;
        }
    }
    else
    {
        if ( NULL == pClassPropertyManager )
        {
            return -1;
        }
        pProperty = pClassPropertyManager->GetElement( strPropertyName );
        if ( NULL == pProperty )
        {
            return -1;
        }
    }

    if ( "Player" == strClassName )
    {
        if ( bTable )
        {
            if ( pRecord->GetPublic() )
            {
                GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
            }
            else if ( pRecord->GetPrivate() )
            {
                valueObject.Add( self );
            }
        }
        else
        {
            if ( pProperty->GetPublic() )
            {
                GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
            }
            else if ( pProperty->GetPrivate() )
            {
                valueObject.Add( self );
            }
        }
        //һ����Ҷ����㲥
        return valueObject.GetCount();
    }

    //�������,NPC�͹������
    if ( bTable )
    {
        if ( pRecord->GetPublic() )
        {
            //�㲥���ͻ����Լ����ܱ���
            GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
        }
    }
    else
    {
        if ( pProperty->GetPublic() )
        {
            //�㲥���ͻ����Լ����ܱ���
            GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
        }
    }

    return valueObject.GetCount();
}

int NFCGameServerNet_ServerModule::GetBroadCastObject( const int nObjectContainerID, const int nGroupID, NFIDataList& valueObject )
{
    NFCDataList valContainerObjectList;
    m_pKernelModule->GetGroupObjectList( nObjectContainerID, nGroupID, valContainerObjectList );
    for ( int i  = 0; i < valContainerObjectList.GetCount(); i++ )
    {
        const std::string& strObjClassName = m_pKernelModule->GetPropertyString( valContainerObjectList.Object( i ), "ClassName" );
        if ( "Player" == strObjClassName )
        {
            valueObject.Add( valContainerObjectList.Object( i ) );
        }
    }

    return valueObject.GetCount();
}

int NFCGameServerNet_ServerModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {
        //SaveDataToNoSql( self, true );
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self.nData64, "Player Offline", "");
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
    {
        //LoadDataFormNoSql( self );
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
    {
        m_pEventProcessModule->AddEventCallBack( self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, this, &NFCGameServerNet_ServerModule::OnSwapSceneResultEvent );
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

int NFCGameServerNet_ServerModule::OnReturnEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnMoveEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnUseSkillResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnSwapSceneResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
    if ( var.GetCount() != 7 ||
        !var.TypeEx(TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, 
        TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_FLOAT,
        TDATA_TYPE::TDATA_FLOAT, TDATA_TYPE::TDATA_FLOAT, TDATA_TYPE::TDATA_UNKNOWN)
        )
    {
        return 1;
    }

    NFIDENTID ident = var.NumberVal<NFINT64>( 0 );
    int nType = var.NumberVal<int>( 1 );
    int nTargetScene = var.NumberVal<int>( 2 );
    int nTargetGroupID = var.NumberVal<int>( 3 );
    float fX = var.NumberVal<float>( 4 );
    float fY = var.NumberVal<float>( 5 );
    float fZ = var.NumberVal<float>( 6 );

    NFMsg::ReqAckSwapScene xSwapScene;
    xSwapScene.set_transfer_type( NFMsg::ReqAckSwapScene::EGameSwapType::ReqAckSwapScene_EGameSwapType_EGST_NARMAL );
    xSwapScene.set_scene_id( nTargetScene );
    xSwapScene.set_line_id( nTargetGroupID );

    BaseData* pData = mRoleBaseData.GetElement(self);
    if (pData)
    {
        ServerData* pProxyData = mProxyMap.GetElement(pData->nGateID);
        if (pProxyData)
        {
            SendMsgPB(NFMsg::EGMI_ACK_SWAP_SCENE, xSwapScene, pProxyData->nFD, pData->nFD);
        }
    }

    return 0;
}

int NFCGameServerNet_ServerModule::OnChatResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
    return 0;
}

void NFCGameServerNet_ServerModule::OnReqiureRoleListProcess( const NFIPacket& msg )
{
    //fd
    int64_t nPlayerID = 0;
    NFMsg::ReqRoleList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
//     NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
//     pData->set_career(0);
//     pData->set_sex(0);
//     pData->set_race(0);
//     pData->set_noob_name("test_role");
//     pData->set_game_id(xMsg.game_id());  
//     pData->set_role_level(0);
//     pData->set_delete_time(0);
//     pData->set_reg_time(0);
//     pData->set_last_offline_time(0);
//     pData->set_last_offline_ip(0);
//     pData->set_view_record("");


    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, msg.GetFd(), nPlayerID);
}

void NFCGameServerNet_ServerModule::OnCreateRoleGameProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
    NFMsg::ReqCreateRole xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
    NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
    pData->set_career(xMsg.career());
    pData->set_sex(xMsg.sex());
    pData->set_race(xMsg.race());
    pData->set_noob_name(xMsg.noob_name());
    pData->set_game_id(xMsg.game_id());  
    pData->set_role_level(1);
    pData->set_delete_time(0);
    pData->set_reg_time(0);
    pData->set_last_offline_time(0);
    pData->set_last_offline_ip(0);
    pData->set_view_record("");

    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, msg.GetFd(), nPlayerID);
}

void NFCGameServerNet_ServerModule::OnDeleteRoleGameProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
    NFMsg::ReqDeleteRole xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, msg.GetFd(), nPlayerID);
}

void NFCGameServerNet_ServerModule::OnClienSwapSceneProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
    NFMsg::ReqAckSwapScene xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    NFIDENTID* pIdent = mRoleFDData.GetElement(nPlayerID);
    if (pIdent)
    {
        NFCDataList var;
        var << *pIdent;
        var << xMsg.transfer_type();
        var << xMsg.scene_id();
        var << xMsg.line_id();

        m_pEventProcessModule->DoEvent( *pIdent, NFED_ON_CLIENT_ENTER_SCENE, var );
    }
}

void NFCGameServerNet_ServerModule::OnClienUseSkill( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienMove( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienCommand( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienAcceptTask( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienPushTask( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienPushCustom( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienPickItem( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienChatProcess( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienUseItem( const NFIPacket& msg )
{

}

int NFCGameServerNet_ServerModule::OnProxyServerRegisteredProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mProxyMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mProxyMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Proxy Registered");
    }

    return 0;
}

int NFCGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mProxyMap.RemoveElement(pData->server_id());
        if (pServerData)
        {
            delete pServerData;
            pServerData = NULL;
        }

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Proxy UnRegistered");
    }
    return 0;
}

int NFCGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mProxyMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mProxyMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Proxy Registered");
    }

    return 0;
}

void NFCGameServerNet_ServerModule::OnClienGMProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
    NFMsg::ReqCommand xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    NFIDENTID* pIdent = mRoleFDData.GetElement(nPlayerID);
    NFIDENTID* pControl = mRoleFDData.GetElement(xMsg.control_id());
    if (!pIdent || !pControl)
    {
        return;
    }

    switch (xMsg.command_id())
    {
    case NFMsg::ReqCommand_EGameCommandType_EGCT_MODIY_PROPERTY:
        {
            const std::string& strPropertyName = xMsg.command_str_value();
            const int nValue = xMsg.command_value();
            m_pKernelModule->SetPropertyInt(*pControl, strPropertyName, nValue);
        }
        break;
    default:
        break;
    }
}
