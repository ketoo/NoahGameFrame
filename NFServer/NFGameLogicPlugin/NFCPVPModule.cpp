// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------

#include "NFCPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCPVPModule::Init()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    return true;
}


bool NFCPVPModule::Shut()
{
    return true;
}

bool NFCPVPModule::Execute()
{
    
    return true;
}

bool NFCPVPModule::AfterInit()
{
	FindAllTileScene();

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_OPPNENT, this, &NFCPVPModule::OnSearchOppnent)) { return false; }
	
    return true;
}



void NFCPVPModule::OnSearchOppnent(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSearchOppnent);
	//find a tile map and swap scene

	int nSceneID = RandomTileScene();
	std::string strTileData;
	if (m_pPlayerRedisModule->LoadPlayerTileRandom(nSceneID, strTileData))
	{
		NFMsg::AckMiningTitle xTileData;
		if (xTileData.ParseFromString(strTileData))
		{
			//tell client u should load resources
			NFMsg::AckSearchOppnent xAckData;
			xAckData.set_scene_id(nSceneID);
			m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, nSockIndex);

			//tell client u shoud adjust tile
			m_pNetModule->SendMsgPB(NFMsg::EGEC_ACK_MINING_TITLE, xTileData, nSockIndex);

			return;
		}
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "ERROR TO FIND A OPPNENT!", "",  __FUNCTION__, __LINE__);
}

void NFCPVPModule::FindAllTileScene()
{
	//Tile
	//mxTileSceneIDList
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		NFList<std::string>& strIdList = xLogicClass->GetIdList();
		std::string strId;
		for (bool bRet = strIdList.First(strId); bRet; bRet = strIdList.Next(strId))
		{
			const int nServerType = m_pElementModule->GetPropertyInt(strId, NFrame::Scene::Tile());
			if (nServerType == 1)
			{
				mxTileSceneIDList.push_back(lexical_cast<int>(strId));
			}
		}
	}
}

void NFCPVPModule::InitAllTileSceneRobot()
{
	for (int i = 0; i < mxTileSceneIDList.size(); ++i)
	{
		int nSceneID = mxTileSceneIDList[i];

	}
}

int NFCPVPModule::RandomTileScene()
{
	return mxTileSceneIDList.at(m_pKernelModule->Random(0, mxTileSceneIDList.size()));
}
