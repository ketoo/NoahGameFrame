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

#include "NFExecutor.h"

void NFCreateObjectExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
	const int sceneID = GetInputArg(NFExecuterCreateObjectInputArg::SceneID)->GetInt();
	const int groupID = GetInputArg(NFExecuterCreateObjectInputArg::GroupID)->GetInt();
	const std::string& className = GetInputArg(NFExecuterCreateObjectInputArg::ClassName)->GetString();
	const std::string& configID = GetInputArg(NFExecuterCreateObjectInputArg::ConfigID)->GetString();
	const NFVector3 pos = GetInputArg(NFExecuterCreateObjectInputArg::Position)->GetVector3();

	NFDataList dataList;
	dataList << NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::Position);
	dataList << pos;

	auto object = m_pKernelModule->CreateObject(NFGUID(), sceneID, groupID, className, configID, dataList);
	if (object)
	{
		auto outputArg = GetOutputArg(NFExecuterCreateObjectOutputputArg::ObjectID);
		if (outputArg)
		{
			outputArg->SetObject(object->Self());
		}
	}
}

NF_SHARE_PTR<NFIOData> NFCreateObjectExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterCreateObjectOutputputArg::NextNode);
}

void NFDestroyObjectExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
	const int sceneID = GetInputArg(NFExecuterCreateObjectInputArg::SceneID)->GetInt();
	const int groupID = GetInputArg(NFExecuterCreateObjectInputArg::GroupID)->GetInt();
	const std::string& className = GetInputArg(NFExecuterCreateObjectInputArg::ClassName)->GetString();
	const std::string& configID = GetInputArg(NFExecuterCreateObjectInputArg::ConfigID)->GetString();
	const NFVector3 pos = GetInputArg(NFExecuterCreateObjectInputArg::Position)->GetVector3();

	NFDataList dataList;
	dataList << NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::Position);
	dataList << pos;

	m_pKernelModule->CreateObject(NFGUID(), sceneID, groupID, className, configID, dataList);

}

NF_SHARE_PTR<NFIOData> NFDestroyObjectExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterDestroyObjectOutputputArg::NextNode);
}

void NFGameEventExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
	const NFGUID objectID = GetInputArg(NFExecuterGameEventInputArg::ObjectID)->GetObject();
	const int eventID = GetInputArg(NFExecuterGameEventInputArg::EventID)->GetInt();
	//const int eventID = GetInputArg(NFExecuterGameEventInputArg::Dctionary)->getd();

	m_pEventModule->DoEvent(objectID, eventID, NFDataList::Empty());
}

NF_SHARE_PTR<NFIOData> NFGameEventExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterGameEventOutputArg::NextNode);
}

void NFMoveObjectExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFMoveObjectExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterMoveObjectOutputArg::NextNode);
}

void NFEnterSceneExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFEnterSceneExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterEnterSceneOutputArg::NextNode);
}

void NFAddHeartBeatExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFAddHeartBeatExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterAddHeartBeatOutputArg::NextNode);
}

void NFAttackObjectExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFAttackObjectExecutor::FindOutputNodeIOData()
{
   // return GetOutputArg(NFExecuterAddHeartBeatOutputArg::NextNode);
    return NF_SHARE_PTR<NFIOData>();
}

void NFSleepExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFSleepExecutor::FindOutputNodeIOData()
{
    return NF_SHARE_PTR<NFIOData>();
}

void NFSendMessageExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFSendMessageExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterSendMessageOutputArg::NextNode);
}
