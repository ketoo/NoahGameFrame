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

#include "NFMonitor.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

void NFGameEventMonitor::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	PrepareInputParameterData(GetInputArg(NFGameEventMonitorInputArg::EventID), runTimeOnwer);
}

void NFGameEventMonitor::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	NF_SHARE_PTR<NFIOData> inputEventID = GetInputArg(NFGameEventMonitorInputArg::EventID);
	//NF_SHARE_PTR<NFIOData> inputEventParameter = GetInputArg(NFGameEventMonitorInputArg::Dictionary);

	NF_SHARE_PTR<NFIOData> outputNextNode = GetOutputArg(NFGameEventMonitorOutputArg::NextNode);
	NF_SHARE_PTR<NFIOData> outputEventID = GetOutputArg(NFGameEventMonitorOutputArg::EventID);
	NF_SHARE_PTR<NFIOData> outputObject = GetOutputArg(NFGameEventMonitorOutputArg::ObjectID);
	//NF_SHARE_PTR<NFIOData> outputDictionary = GetOutputArg(NFGameEventMonitorOutputArg::Dictionary);

	outputEventID->varData.SetInt(inputEventID->varData.GetInt());
	outputObject->varData.SetObject(runTimeOnwer);
	//outputDictionary->dictionaryData = inputEventParameter->dictionaryData;

}

NF_SHARE_PTR<NFIOData> NFGameEventMonitor::FindOutputNodeIOData()
{
	return GetOutputArg(NFGameEventMonitorOutputArg::NextNode);
}

void NFNetworkEventMonitor::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	PrepareInputParameterData(GetInputArg(NFNetworkEventMonitorInputArg::EventID), runTimeOnwer);
}

void NFNetworkEventMonitor::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	NF_SHARE_PTR<NFIOData> inputEventID = GetInputArg(NFNetworkEventMonitorInputArg::EventID);

	NF_SHARE_PTR<NFIOData> outputNextNode = GetOutputArg(NFNetworkEventMonitorOutputArg::NextNode);
	NF_SHARE_PTR<NFIOData> outputObjectID = GetOutputArg(NFNetworkEventMonitorOutputArg::ObjectID);
	NF_SHARE_PTR<NFIOData> outputEventID = GetOutputArg(NFNetworkEventMonitorOutputArg::EventID);

	outputEventID->varData.SetInt(inputEventID->varData.GetInt());
	outputObjectID->varData.SetObject(runTimeOnwer);
}

NF_SHARE_PTR<NFIOData> NFNetworkEventMonitor::FindOutputNodeIOData()
{
	return GetOutputArg(NFNetworkEventMonitorOutputArg::NextNode);
}

void NFNetworkMsgEventMonitor::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	PrepareInputParameterData(GetInputArg(NFNetworkMsgMonitorInputArg::NetMsgID), runTimeOnwer);
}

void NFNetworkMsgEventMonitor::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	NF_SHARE_PTR<NFIOData> inputMsgID = GetInputArg(NFNetworkMsgMonitorInputArg::NetMsgID);

	NF_SHARE_PTR<NFIOData> outputNextNode = GetOutputArg(NFNetworkkMsgMonitorOutputArg::NextNode);
	NF_SHARE_PTR<NFIOData> outputObjectID = GetOutputArg(NFNetworkkMsgMonitorOutputArg::ObjectID);
	NF_SHARE_PTR<NFIOData> outputMsgID = GetOutputArg(NFNetworkkMsgMonitorOutputArg::NetMsgID);
	NF_SHARE_PTR<NFIOData> outputDictionary = GetOutputArg(NFNetworkkMsgMonitorOutputArg::Dictionary);

	outputObjectID->varData.SetObject(runTimeOnwer);
	outputMsgID->varData.SetInt(inputMsgID->varData.GetInt());
	//fake data now
	//outputDictionary->dictionaryData = std::map<>;
}

NF_SHARE_PTR<NFIOData> NFNetworkMsgEventMonitor::FindOutputNodeIOData()
{
	return GetOutputArg(NFNetworkkMsgMonitorOutputArg::NextNode);
}

void NFObjectEventMonitor::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	PrepareInputParameterData(GetInputArg(NFMonitorObjectEventInputArg::ClassName), runTimeOnwer);
	PrepareInputParameterData(GetInputArg(NFMonitorObjectEventInputArg::ClassEvent), runTimeOnwer);
}

void NFObjectEventMonitor::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	NF_SHARE_PTR<NFIOData> outputNextNode = GetOutputArg(NFMonitorObjectEventOutputArg::NextNode);

	NF_SHARE_PTR<NFIOData> outputObjectID = GetOutputArg(NFMonitorObjectEventOutputArg::ObjectID);
	NF_SHARE_PTR<NFIOData> outputClassName = GetOutputArg(NFMonitorObjectEventOutputArg::ClassName);
	NF_SHARE_PTR<NFIOData> outputConfigID = GetOutputArg(NFMonitorObjectEventOutputArg::ConfigID);
	NF_SHARE_PTR<NFIOData> outputSceneID = GetOutputArg(NFMonitorObjectEventOutputArg::SceneID);
	NF_SHARE_PTR<NFIOData> outputGroupID = GetOutputArg(NFMonitorObjectEventOutputArg::GroupID);
	NF_SHARE_PTR<NFIOData> outputPosition = GetOutputArg(NFMonitorObjectEventOutputArg::Position);

	const std::string& className = m_pKernelModule->GetPropertyString(runTimeOnwer, NFrame::IObject::ClassName());
	const std::string& configID = m_pKernelModule->GetPropertyString(runTimeOnwer, NFrame::IObject::ConfigID());
	const int sceneID = m_pKernelModule->GetPropertyInt(runTimeOnwer, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(runTimeOnwer, NFrame::IObject::GroupID());
	const NFVector3& position = m_pKernelModule->GetPropertyVector3(runTimeOnwer, NFrame::IObject::Position());

	outputObjectID->varData.SetObject(runTimeOnwer);
	outputClassName->varData.SetString(className);
	outputConfigID->varData.SetString(configID);
	outputSceneID->varData.SetInt(sceneID);
	outputGroupID->varData.SetInt(groupID);
	outputPosition->varData.SetVector3(position);
}

NF_SHARE_PTR<NFIOData> NFObjectEventMonitor::FindOutputNodeIOData()
{
	return GetOutputArg(NFMonitorObjectEventOutputArg::NextNode);
}

void NFPropertyEventMonitor::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	PrepareInputParameterData(GetInputArg(NFMonitorPropertyEventInputArg::ClassName), runTimeOnwer);
	PrepareInputParameterData(GetInputArg(NFMonitorPropertyEventInputArg::PropName), runTimeOnwer);
}

void NFPropertyEventMonitor::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	NF_SHARE_PTR<NFIOData> className = GetInputArg(NFMonitorPropertyEventInputArg::ClassName);
	NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFMonitorPropertyEventInputArg::PropName);

	NF_SHARE_PTR<NFIOData> outputSelf = GetOutputArg(NFMonitorPropertyEventOutputArg::ObjectID);
	NF_SHARE_PTR<NFIOData> outputPropertyName = GetOutputArg(NFMonitorPropertyEventOutputArg::PropName);

	outputSelf->varData.SetObject(runTimeOnwer);
	outputPropertyName->varData = propertyName->varData;
}

NF_SHARE_PTR<NFIOData> NFPropertyEventMonitor::FindOutputNodeIOData()
{
	return GetOutputArg(NFMonitorPropertyEventOutputArg::NextNode);
}

void NFRecordEventMonitor::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	PrepareInputParameterData(GetInputArg(NFMonitorRecordEventInputArg::ClassName), runTimeOnwer);
	PrepareInputParameterData(GetInputArg(NFMonitorRecordEventInputArg::RecordName), runTimeOnwer);
	PrepareInputParameterData(GetInputArg(NFMonitorRecordEventInputArg::OperateType), runTimeOnwer);
}

void NFRecordEventMonitor::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	/*
	get the parameteres from VM (row and col)

	NextNode,
		ClassName,
		RecordName,
		Row,
		Col,
	*/


}

NF_SHARE_PTR<NFIOData> NFRecordEventMonitor::FindOutputNodeIOData()
{
	return GetOutputArg(NFMonitorRecordEventOutArg::NextNode);
}

void NFSceneEventMonitor::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	PrepareInputParameterData(GetInputArg(NFMonitorSceneEventInputArg::ClassName), runTimeOnwer);
	PrepareInputParameterData(GetInputArg(NFMonitorSceneEventInputArg::SceneID), runTimeOnwer);
	PrepareInputParameterData(GetInputArg(NFMonitorSceneEventInputArg::OperateType), runTimeOnwer);
}

void NFSceneEventMonitor::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
	NF_SHARE_PTR<NFIOData> intputClassName = GetOutputArg(NFMonitorSceneEventInputArg::ClassName);
	NF_SHARE_PTR<NFIOData> intputSceneID = GetOutputArg(NFMonitorSceneEventInputArg::SceneID);
	NF_SHARE_PTR<NFIOData> intputType = GetOutputArg(NFMonitorSceneEventInputArg::OperateType);

	NF_SHARE_PTR<NFIOData> outputClassName = GetOutputArg(NFMonitorSceneEventOutArg::ClassName);
	NF_SHARE_PTR<NFIOData> outputSceneID = GetOutputArg(NFMonitorSceneEventOutArg::SceneID);
	NF_SHARE_PTR<NFIOData> outputGroupID = GetOutputArg(NFMonitorSceneEventOutArg::GroupID);
	NF_SHARE_PTR<NFIOData> outputObjectID = GetOutputArg(NFMonitorSceneEventOutArg::ObjectID);

	outputClassName->varData.SetString(intputClassName->varData.GetString());
	outputSceneID->varData.SetInt(intputSceneID->varData.GetInt());
	outputGroupID->varData.SetInt(m_pKernelModule->GetPropertyInt(runTimeOnwer, NFrame::IObject::GroupID()));
	outputObjectID->varData.SetObject(runTimeOnwer);
}

NF_SHARE_PTR<NFIOData> NFSceneEventMonitor::FindOutputNodeIOData()
{
	return GetOutputArg(NFMonitorSceneEventOutArg::NextNode);
}
