/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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


#include "NFFriendModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFFriendModule::Init()
{
	return true;
}

bool NFFriendModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	

	return true;
}

bool NFFriendModule::IsEnemy(const NFGUID & self, const NFGUID & other)
{
	if (self == other)
	{
		return false;
	}
	if (!m_pKernelModule->ExistObject(self)
		|| !m_pKernelModule->ExistObject(other))
	{
		return false;
	}

	const std::string& selfClassName = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
	NFGUID selfCampID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::Camp());
	NFGUID selfMasterID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::MasterID());
	long selfHP = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::HP());

	const std::string& enemyClassName = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
	NFGUID enemyCampID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::Camp());
	NFGUID enemyMasterID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::MasterID());
	long enemyHP = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::HP());

	if (enemyHP <= 0)
	{
		return false;
	}

	if (selfMasterID == other
		|| selfMasterID == enemyMasterID)
	{
		return false;
	}

	if (!selfCampID.IsNull() || !enemyCampID.IsNull())
	{
		if (selfCampID == enemyCampID)
		{
			return false;
		}
	}

	return true;
}

bool NFFriendModule::Shut()
{

	return true;
}

bool NFFriendModule::Execute()
{
	return true;
}
