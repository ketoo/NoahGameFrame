// -------------------------------------------------------------------------
//    @FileName				:    NFCCreateRoleModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-03-29
//    @Module               :    NFCCreateRoleModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCCreateRoleModule.h"

bool NFCCreateRoleModule::Init()
{
/*
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, OnLoadRoleBeginEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_DATABASE_SERVER_LOADROE_FINAL_RESULTS, OnLoadRoleFinalEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_CREATEROLE, OnCreateRoleEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_DELETEROLE, OnDeleteRoleEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LEAVE_GAME, OnAcountDisConnectEvent);
	*/
    return true;
}

bool NFCCreateRoleModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pGameLogicModule = pPluginManager->FindModule<NFIGameLogicModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	///

	return true;
}

bool NFCCreateRoleModule::ReadyExecute()
{
	m_pGameServerNet_ServerModule->GetNetModule()->RemoveReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST);
	m_pGameServerNet_ServerModule->GetNetModule()->RemoveReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE);
	m_pGameServerNet_ServerModule->GetNetModule()->RemoveReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE);

	m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST, this, &NFCCreateRoleModule::OnReqiureRoleListProcess);
	m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE, this, &NFCCreateRoleModule::OnCreateRoleGameProcess);
	m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE, this, &NFCCreateRoleModule::OnDeleteRoleGameProcess);

	return true;
}

bool NFCCreateRoleModule::Shut()
{
    return true;
}

bool NFCCreateRoleModule::Execute()
{
    return true;
}

void NFCCreateRoleModule::OnReqiureRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//fd
	NFGUID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!m_pGameServerNet_ServerModule->GetNetModule()->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	m_pGameServerNet_ServerModule->GetNetModule()->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFCCreateRoleModule::OnCreateRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqCreateRole xMsg;
	if (!m_pGameServerNet_ServerModule->GetNetModule()->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (!pNosqlDriver)
	{
		return;
	}

	if (pNosqlDriver->Exists(xMsg.noob_name()))
	{
		//name repeated---> show a message box
		return;
	}

	if (pNosqlDriver->Exists(xMsg.noob_name()))
	{
		//name repeated---> show a message box
		return;
	}


	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
	pData->mutable_id()->CopyFrom(NFINetModule::NFToPB(m_pKernelModule->CreateGUID()));
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

	m_pGameServerNet_ServerModule->GetNetModule()->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFCCreateRoleModule::OnDeleteRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqDeleteRole xMsg;
	if (!m_pGameServerNet_ServerModule->GetNetModule()->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}


	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	m_pGameServerNet_ServerModule->GetNetModule()->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nPlayerID);
}
/*

int NFCCreateRoleModule::OnLoadRoleFinalEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (9 != var.GetCount())
    {
        return -1;
    }

    
    const char* pstrAccount = var.StringVal(0);
    NFCValueList valueInfo;
    valueInfo << pstrAccount;

    int nCount = 0;
    for (int i = 1; i <= 7; i += 2)
    {
        const char* pstrRoleName = var.StringVal(i);
        if (strlen(pstrRoleName) > 0)
        {
            
            NFCValueList varHallObjectList;
            NFCValueList varHalvalueInfo;
            varHalvalueInfo << pstrRoleName;
            int nHallObjectCount = m_pKernelModule->GetObjectByProperty(mnRoleHallContainer, "RoleName", varHalvalueInfo, varHallObjectList);
            if (nHallObjectCount > 0)
            {

                for (int j = 0; j < varHallObjectList.GetCount(); j++)
                {
                    m_pKernelModule->LogErrorObject(varHallObjectList.ObjectVal(j));
                    m_pKernelModule->DestroyObject(varHallObjectList.ObjectVal(j));
                }
            }

            GTProperty* gtPproperty = m_pElementInfoModule->GetPropertyManager("Scene0")->GetElement("RelivePos");
            const char* pstrRelivePos = gtPproperty->QueryString();
            NFCValueList valueRelivePos(pstrRelivePos, ",");

            char szConfigIindex[MAX_PATH] = { 0 };
            sprintf(szConfigIindex, "%d", var.IntVal(i + 1));

            NFCValueList arg;
            arg << "Account" << pstrAccount;
            arg << "RoleName" << pstrRoleName;
            arg << "SceneID" << mnRoleHallContainer;
            arg << "X" << atof(valueRelivePos.StringVal(0));
            arg << "Y" << atof(valueRelivePos.StringVal(1));
            arg << "Z" << atof(valueRelivePos.StringVal(2));
            m_pKernelModule->CreateObject(0, mnRoleHallContainer, 0, "Player", szConfigIindex, arg);

            nCount++;
        }
    }

    char szInfo[MAX_PATH] = { 0 };
    sprintf(szInfo, "Load data final, %s: have %d role.", pstrAccount, nCount);
    m_pKernelModule->LogInfo(szInfo);

    return 0;
}

int NFCCreateRoleModule::OnCreateRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (6 != var.GetCount())
    {
        return 0;
    }

    
    const char* pstrAccountName = var.StringVal(0);
    const char* pstrRoleName = var.StringVal(1);
    int nRoleSex = var.IntVal(2);
    int nRoleJob = var.IntVal(3);
    int nRoleRace = var.IntVal(4);
    int nRoleCamp = var.IntVal(5);

    NFCValueList roleLlist;
    if (m_pNoSqlModule->QueryAccountRoleList(pstrAccountName, roleLlist) >= 4)
        //if (m_pDataBaseModule->QueryAccountRoleList(pstrAccountName, roleLlist) >= 4)
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////////////
    if (m_pNoSqlModule->CreateRole(pstrAccountName, pstrRoleName) <= 0)
        //if (m_pDataBaseModule->CreateRole(pstrAccountName, pstrRoleName) <= 0)
    {
        return 0;
    }

    NFCValueList varPropertyKeyList;
    NFCValueList varPropertyValueList;

    varPropertyKeyList << "RoleName"
                       << "Sex"
                       << "Job"
                       << "Race"
                       << "Camp"
                       << "SceneID"
                       << "LastContainerID"
                       << "ClassName";

    varPropertyValueList << pstrRoleName
                         << nRoleSex
                         << nRoleJob
                         << nRoleRace
                         << nRoleCamp
                         << mnRoleHallContainer
                         << 1
                         << "Player";

    int nConfigName = nRoleJob + nRoleSex * 10;
    char szConfigName[MAX_PATH] = { 0 };
    sprintf(szConfigName, "%d", nConfigName);

    
    GTPropertyManager* pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager(szConfigName);
    if (pConfigPropertyManager)
    {
        GTProperty* pConfigPropertyInfo = pConfigPropertyManager->First();
        while (pConfigPropertyInfo)
        {
            
            if (!pConfigPropertyInfo->GetSave())
            {
                pConfigPropertyInfo = pConfigPropertyManager->Next();
                continue;
            }
            
            const NFIValueList& valueList = pConfigPropertyInfo->GetValue();


            if (0 != strcmp(pConfigPropertyInfo->GetKey(), "RoleName")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "Sex")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "Job")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "Race")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "Camp")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "Account")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "SceneID")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "LastContainerID")
                && 0 != strcmp(pConfigPropertyInfo->GetKey(), "ClassName"))
            {
                varPropertyKeyList << pConfigPropertyInfo->GetKey();
                varPropertyValueList.Append(valueList, 0, 1);
            }

            pConfigPropertyInfo = pConfigPropertyManager->Next();
        }
    }
    varPropertyKeyList << "RoleName";
    varPropertyValueList << pstrRoleName;
    varPropertyKeyList << "Sex";
    varPropertyValueList << nRoleSex;
    varPropertyKeyList << "Job";
    varPropertyValueList << nRoleJob;
    varPropertyKeyList << "Race";
    varPropertyValueList << nRoleRace;
    varPropertyKeyList << "Camp";
    varPropertyValueList << nRoleCamp;
    varPropertyKeyList << "SceneID";
    varPropertyValueList << mnRoleHallContainer;
    varPropertyKeyList << "LastContainerID";
    varPropertyValueList << 1;
    varPropertyKeyList << "ClassName";
    varPropertyValueList << "Player";
    varPropertyKeyList << "Level";
    varPropertyValueList << 1;

    m_pNoSqlModule->SetRoleProperty(pstrRoleName, varPropertyKeyList, varPropertyValueList);

    //////////////////////////////////////////////////////////////////////////
    GTProperty* gtPproperty = m_pElementInfoModule->GetPropertyManager("Scene0")->GetElement("RelivePos");
    const char* pstrRelivePos = gtPproperty->QueryString();
    NFCValueList valueRelivePos(pstrRelivePos, ",");

    NFCValueList arg;
    arg << "Account" << pstrAccountName;
    arg << "RoleName" << pstrRoleName;
    arg << "SceneID" << mnRoleHallContainer;
    arg << "X" << atof(valueRelivePos.StringVal(0));
    arg << "Y" << atof(valueRelivePos.StringVal(1));
    arg << "Z" << atof(valueRelivePos.StringVal(2));
    m_pKernelModule->CreateObject(0, mnRoleHallContainer, 0, "Player", szConfigName, arg);
    
    //m_pEventProcessModule->DoEvent(0, NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, NFCValueList() << pstrAccountName);

    return 0;
}

int NFCCreateRoleModule::OnDeleteRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (2 != var.GetCount())
    {
        return 0;
    }

    const char* pstrAccountName = var.StringVal(0);
    const char* pstrRoleName = var.StringVal(1);

    NFCValueList valObjctList;
    m_pKernelModule->GetObjectByProperty(mnRoleHallContainer, "RoleName", NFCValueList() << pstrRoleName, valObjctList);
    if (valObjctList.GetCount() == 1)
    {
        m_pNoSqlModule->DeleteRole(pstrAccountName, pstrRoleName);
        //m_pDataBaseModule->DeleteRole(pstrAccountName, pstrRoleName);
        m_pKernelModule->DestroyObject(valObjctList.ObjectVal(0));
        //m_pEventProcessModule->DoEvent( 0, NFED_ON_CLIENT_DELETEROLE_RESULTS, NFCValueList() << pstrAccountName << pstrRoleName );
    }

    return 0;
}

int NFCCreateRoleModule::OnAcountDisConnectEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (var.GetCount() == 3)
    {
        const char* pstrAccount = var.StringVal(0);
        const char* pstrRoleName = var.StringVal(1);
        NFIDENTID ident = var.ObjectVal(2);

        if (strlen(pstrAccount) > 0)
        {
            if (m_pGameLogicModule->GetGameID() == 1)
            {

                if (!ident.IsNull())
                {
                    m_pKernelModule->DestroyObject(ident);
                }
                else
                {
                    NFCValueList varHallObjectList;
                    NFCValueList varHalvalueInfo;
                    varHalvalueInfo << pstrAccount;

                    int nHallObjectCount = m_pKernelModule->GetObjectByProperty(mnRoleHallContainer, "Account", varHalvalueInfo, varHallObjectList);
                    for (int i  = 0; i < varHallObjectList.GetCount(); i++)
                    {
                        NFIDENTID identHall = varHallObjectList.ObjectVal(i);
                        if (ident.nSerial == m_pGameLogicModule->GetGameID()
                            && m_pKernelModule->GetObject(identHall))
                        {
                            m_pKernelModule->DestroyObject(identHall);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int NFCCreateRoleModule::OnLoadRoleBeginEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    
    const char* pstrAccount = var.StringVal(0);
    NFCValueList roleLlist;
    if (m_pNoSqlModule->QueryAccountRoleList(pstrAccount, roleLlist) > 0)
    {
        for (int i = 0; i < roleLlist.GetCount(); i++)
        {
            const char* pstrRoleName = roleLlist.StringVal(i);
            if (strlen(pstrRoleName) > 0)
            {
                
                NFCValueList varHallObjectList;
                NFCValueList varHalvalueInfo;
                varHalvalueInfo << pstrRoleName;
                int nHallObjectCount = m_pKernelModule->GetObjectByProperty(mnRoleHallContainer, "RoleName", varHalvalueInfo, varHallObjectList);
                if (nHallObjectCount > 0)
                {
                    for (int j = 0; j < varHallObjectList.GetCount(); j++)
                    {
                        m_pKernelModule->LogErrorObject(varHallObjectList.ObjectVal(j));
                        m_pKernelModule->DestroyObject(varHallObjectList.ObjectVal(j));
                    }
                }

                GTProperty* gtPproperty = m_pElementInfoModule->GetPropertyManager("Scene0")->GetElement("RelivePos");
                const char* pstrRelivePos = gtPproperty->QueryString();
                NFCValueList valueRelivePos(pstrRelivePos, ",");

                char szConfigIindex[MAX_PATH] = { 0 };
                sprintf(szConfigIindex, "%d", var.IntVal(i + 1));

                NFCValueList arg;
                arg << "Account" << pstrAccount;
                arg << "RoleName" << pstrRoleName;
                arg << "SceneID" << mnRoleHallContainer;
                arg << "X" << atof(valueRelivePos.StringVal(0));
                arg << "Y" << atof(valueRelivePos.StringVal(1));
                arg << "Z" << atof(valueRelivePos.StringVal(2));
                m_pKernelModule->CreateObject(0, mnRoleHallContainer, 0, "Player", szConfigIindex, arg);
            }
        }
    }

    return 0;
}
*/