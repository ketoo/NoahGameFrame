/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include <NFComm/NFPluginModule/NFIPropertyModule.h>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFNPCRefreshModule.h"

bool NFNPCRefreshModule::Init()
{
    return true;
}


bool NFNPCRefreshModule::Shut()
{
    return true;
}

bool NFNPCRefreshModule::Execute()
{
    return true;
}

bool NFNPCRefreshModule::AfterInit()
{
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFNPCRefreshModule::OnObjectClassEvent);

    return true;
}

int NFNPCRefreshModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
{
    NF_SHARE_PTR<NFIObject> pSelf = m_pKernelModule->GetObject(self);
    if (nullptr == pSelf)
    {
        return 1;
    }

    if (strClassName == NFrame::NPC::ThisName())
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
        {
            const std::string& strConfigIndex = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
			const std::string& strEffectPropertyID = m_pElementModule->GetPropertyString(strConfigIndex, NFrame::NPC::EffectData());
			const int nNPCType = m_pElementModule->GetPropertyInt32(strConfigIndex, NFrame::NPC::NPCType());
			NF_SHARE_PTR<NFIPropertyManager> pSelfPropertyManager = pSelf->GetPropertyManager();

			//effect data
			//normal npc
			NF_SHARE_PTR<NFIPropertyManager> pConfigPropertyManager = m_pElementModule->GetPropertyManager(strEffectPropertyID);
			if (pConfigPropertyManager)
			{
				std::string strProperName;
				for (NFIProperty* pProperty = pConfigPropertyManager->FirstNude(strProperName); pProperty != NULL; pProperty = pConfigPropertyManager->NextNude(strProperName))
				{
					if (pSelfPropertyManager && pProperty->Changed()
						&& strProperName != NFrame::IObject::ID()
						&& strProperName != NFrame::IObject::ConfigID()
						&& strProperName != NFrame::IObject::ClassName()
						&& strProperName != NFrame::IObject::SceneID()
						&& strProperName != NFrame::IObject::GroupID())
					{
						pSelfPropertyManager->SetProperty(pProperty->GetKey(), pProperty->GetValue());
					}
				}
			}

			if (nNPCType == NFMsg::ENPCType::HERO_NPC)
			{
				//star & level
			}
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
			int nHPMax = m_pKernelModule->GetPropertyInt32(self, NFrame::NPC::MAXHP());
            m_pKernelModule->SetPropertyInt(self, NFrame::NPC::HP(), nHPMax);

            m_pKernelModule->AddPropertyCallBack( self, NFrame::NPC::HP(), this, &NFNPCRefreshModule::OnObjectHPEvent );
        }
    }

    return 0;
}

int NFNPCRefreshModule::OnObjectHPEvent( const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
    if ( newVar.GetInt() <= 0 )
    {
        const NFGUID& identAttacker = m_pKernelModule->GetPropertyObject( self, NFrame::NPC::LastAttacker());
        if (!identAttacker.IsNull())
		{
			OnObjectBeKilled(self, identAttacker);
        }

		m_pScheduleModule->AddSchedule( self, "OnNPCDeadDestroyHeart", this, &NFNPCRefreshModule::OnNPCDeadDestroyHeart, 1.0f, 1 );
    }

    return 0;
}

int NFNPCRefreshModule::OnNPCDeadDestroyHeart( const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
    //and create new object
	int nSceneID = m_pKernelModule->GetPropertyInt32( self, NFrame::NPC::SceneID());
	int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::NPC::GroupID());
	int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());

	if (nNPCType == NFMsg::ENPCType::NORMAL_NPC)
	{

		const std::string& strClassName = m_pKernelModule->GetPropertyString( self, NFrame::NPC::ClassName());
		const std::string& strSeedID = m_pKernelModule->GetPropertyString( self, NFrame::NPC::SeedID());
		const std::string& strConfigID = m_pKernelModule->GetPropertyString( self, NFrame::NPC::ConfigID());
		const NFGUID xMasterID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::MasterID());
		const NFGUID camp = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::CampID());
		int refresh = m_pKernelModule->GetPropertyInt32(self, NFrame::NPC::Refresh());

		m_pKernelModule->DestroySelf(self);

		const NFVector3& seedPos = m_pSceneModule->GetSeedPos(nSceneID, strSeedID);
		if (refresh > 0)
		{
			NFDataList arg;
			arg << NFrame::NPC::Position() << seedPos;
			arg << NFrame::NPC::SeedID() << strSeedID;
			arg << NFrame::NPC::MasterID() << xMasterID;
			arg << NFrame::NPC::CampID() << camp;
			arg << NFrame::NPC::Refresh() << refresh;

			m_pKernelModule->CreateObject(NFGUID(), nSceneID, nGroupID, strClassName, strConfigID, arg);
		}
	}
	else
	{
		m_pKernelModule->DestroySelf(self);
	}

    return 0;
}

int NFNPCRefreshModule::OnBuildingDeadDestroyHeart(const NFGUID & self, const std::string & strHeartBeat, const float fTime, const int nCount)
{
	//and create new object
	const std::string& strClassName = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ClassName());
	const std::string& strSeedID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::SeedID());
	const std::string& strConfigID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
	const NFGUID xMasterID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::MasterID());
	int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
	int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::NPC::SceneID());
	int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::NPC::GroupID());

	NFVector3 fSeedPos = m_pKernelModule->GetPropertyVector3(self, NFrame::NPC::Position());

	if (nNPCType == NFMsg::ENPCType::TURRET_NPC)
	{
		m_pKernelModule->DestroySelf(self);

		NFDataList arg;
		arg << NFrame::NPC::Position() << fSeedPos;
		arg << NFrame::NPC::SeedID() << strSeedID;
		arg << NFrame::NPC::MasterID() << xMasterID;

		m_pKernelModule->CreateObject(NFGUID(), nSceneID, nGroupID, strClassName, strConfigID, arg);
	}
	
	return 0;
}

int NFNPCRefreshModule::OnObjectBeKilled( const NFGUID& self, const NFGUID& killer )
{
	if ( m_pKernelModule->GetObject(killer) )
	{
		const int64_t nExp = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::EXP());
		const int64_t nGold = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::Gold());

		m_pPropertyModule->AddExp(killer, nExp);
		m_pPropertyModule->AddGold(killer, nGold);
	}

	return 0;
}