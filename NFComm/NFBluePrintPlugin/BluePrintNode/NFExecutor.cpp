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
	const int sceneID = GetInputArg(NFExecutorCreateObjectInputArg::SceneID)->GetInt();
	const int groupID = GetInputArg(NFExecutorCreateObjectInputArg::GroupID)->GetInt();
	const std::string& className = GetInputArg(NFExecutorCreateObjectInputArg::ClassName)->GetString();
	const std::string& configID = GetInputArg(NFExecutorCreateObjectInputArg::ConfigID)->GetString();
	const NFVector3 pos = GetInputArg(NFExecutorCreateObjectInputArg::Position)->GetVector3();

	NFDataList dataList;
	dataList << NFExecutorCreateObjectInputArg::toString(NFExecutorCreateObjectInputArg::Position);
	dataList << pos;

	auto object = m_pKernelModule->CreateObject(NFGUID(), sceneID, groupID, className, configID, dataList);
	if (object)
	{
		auto outputArg = GetOutputArg(NFExecutorCreateObjectOutputputArg::ObjectID);
		if (outputArg)
		{
			outputArg->SetObject(object->Self());
		}
	}
}

NF_SHARE_PTR<NFIOData> NFCreateObjectExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecutorCreateObjectOutputputArg::NextNode);
}

void NFDestroyObjectExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
	NF_SHARE_PTR<NFIOData> objectID = GetInputArg(NFExecutorDestroyObjectInputArg::ObjectID);
	if (!objectID->GetObject().IsNull())
	{
		auto kernelModule = this->pPluginManager->FindModule<NFIKernelModule>();
		kernelModule->DestroyObject(objectID->GetObject());
	}

}

NF_SHARE_PTR<NFIOData> NFDestroyObjectExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecutorDestroyObjectOutputputArg::NextNode);
}

void NFGameEventExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
	const NFGUID objectID = GetInputArg(NFExecutorGameEventInputArg::ObjectID)->GetObject();
	const int eventID = GetInputArg(NFExecutorGameEventInputArg::EventID)->GetInt();
	//const int eventID = GetInputArg(NFExecutorGameEventInputArg::Dctionary)->getd();

	m_pEventModule->DoEvent(objectID, eventID, NFDataList::Empty());
}

NF_SHARE_PTR<NFIOData> NFGameEventExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecutorGameEventOutputArg::NextNode);
}

void NFMoveObjectExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFMoveObjectExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecutorMoveObjectOutputArg::NextNode);
}

void NFEnterSceneExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFEnterSceneExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecutorEnterSceneOutputArg::NextNode);
}

void NFAddHeartBeatExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFAddHeartBeatExecutor::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecutorAddHeartBeatOutputArg::NextNode);
}

void NFAttackObjectExecutor::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFAttackObjectExecutor::FindOutputNodeIOData()
{
   // return GetOutputArg(NFExecutorAddHeartBeatOutputArg::NextNode);
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
    return GetOutputArg(NFExecutorSendMessageOutputArg::NextNode);
}
