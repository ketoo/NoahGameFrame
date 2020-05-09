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

//#include "Dependencies/googletest-release-1.8.0/googletest/include/gtest/gtest.h"
#include "NFBPVirtualMachine.h"

void NFBPVirtualMachine::StartToProcessMonitor()
{
    for (auto item : mLogicBlock->monitors)
    {
        StartMonitor(item);
    }
}

int NFBPVirtualMachine::GameEventIDCallBack(const NFGUID& objectID, const  NFGUID& monitorID, const int eventID, const const NFMapEx<std::string, NFData>& data)
{
    auto node = m_pBluePrintModule->FindNode(monitorID);
    if (node && node->blueprintType == NFBlueprintType::MONITOR)
    {
        node->Execute(objectID);
    }

    return 0;
}

int NFBPVirtualMachine::NetEventIDCallBack(const NFGUID& objectID, const NFGUID& monitorID, const int eventID, const const NFMapEx<std::string, NFData>& data)
{
	auto node = m_pBluePrintModule->FindNode(monitorID);
	if (node->blueprintType == NFBlueprintType::MONITOR)
	{
		node->Execute(objectID);
	}

	return 0;
}

int NFBPVirtualMachine::NetMsgCallBack(const NFGUID& objectID, const NFGUID& monitorID, const int eventID, const const NFMapEx<std::string, NFData>& data)
{
	auto node = m_pBluePrintModule->FindNode(monitorID);
	if (node->blueprintType == NFBlueprintType::MONITOR)
	{
		node->Execute(objectID);
	}

	return 0;
}

int NFBPVirtualMachine::GameObjectCallBack(const NFGUID& objectID, const NFGUID& monitorID, const int eventID, const const NFMapEx<std::string, NFData>& data)
{

    return 0;
}

int NFBPVirtualMachine::ObjectPropCallBack(const NFGUID& objectID, const NFGUID& monitorID, const int eventID, const const NFMapEx<std::string, NFData>& data)
{
    return 0;
}

int NFBPVirtualMachine::ObjectRecordCallBack(const NFGUID& objectID, const NFGUID& monitorID, const int eventID, const const NFMapEx<std::string, NFData>& data)
{
    return 0;
}

int NFBPVirtualMachine::ObjectSceneEventCallBack(const NFGUID& objectID, const NFGUID& monitorID, const int eventID, const const NFMapEx<std::string, NFData>& data)
{
    return 0;
}

void NFBPVirtualMachine::StartMonitor(NF_SHARE_PTR<NFIMonitor> monitor)
{
    switch (monitor->monitorType)
    {
    case NFMonitorType::GameEvent:
    {
        auto inputEventID = monitor->GetInputArg(NFGameEventMonitorInputArg::toString(NFGameEventMonitorInputArg::EventID));
        if (inputEventID->GetInt() <= 0)
        {
            m_pLogModule->LogError(monitor->name + " The monitor's Input EventID is empty.");
            return;
        }

        try
        {
            int eventID = inputEventID->GetInt();
            m_pBPVMEventModule->RegisterGameEventCallBack(monitor->logicBlockId, eventID, monitor->id, this, &NFBPVirtualMachine::GameEventIDCallBack);
        }
        catch (const std::exception&)
        {
            m_pLogModule->LogError(monitor->name + " The monitor's Input EventID must as a integer: " + inputEventID->ToString());
            return;
        }
    }
        break;
    case NFMonitorType::NetworkEvent:
	{
		auto inputEventID = monitor->GetInputArg(NFNetworkEventMonitorInputArg::toString(NFNetworkEventMonitorInputArg::EventID));
		if (inputEventID->GetInt() <= 0)
		{
			m_pLogModule->LogError(monitor->name + " The monitor's Input EventID is empty.");
			return;
		}

		try
		{
			int eventID = inputEventID->GetInt();
			m_pBPVMEventModule->RegisterNetEventCallBack(monitor->logicBlockId, eventID, monitor->id, this, &NFBPVirtualMachine::NetEventIDCallBack);
		}
		catch (const std::exception&)
		{
			m_pLogModule->LogError(monitor->name + " The monitor's Input EventID must as a integer: " + inputEventID->ToString());
			return;
		}
	}
        break;
    case NFMonitorType::NetworkMsgEvent:
	{
		auto inputEventID = monitor->GetInputArg(NFNetworkMsgMonitorInputArg::toString(NFNetworkMsgMonitorInputArg::NetMsgID));
		if (inputEventID->GetInt() <= 0)
		{
			m_pLogModule->LogError(monitor->name + " The monitor's Input EventID is empty.");
			return;
		}

		try
		{
			int eventID = inputEventID->GetInt();
			m_pBPVMEventModule->RegisterNetMsgEventCallBack(monitor->logicBlockId, eventID, monitor->id, this, &NFBPVirtualMachine::NetMsgCallBack);
		}
		catch (const std::exception&)
		{
			m_pLogModule->LogError(monitor->name + " The monitor's Input EventID must as a integer: " + inputEventID->ToString());
			return;
		}
	}
        break;
    case NFMonitorType::ObjectEvent:
	{
		
	}
        break;
    case NFMonitorType::PropertyEvent:
	{

	}
        break;
    case NFMonitorType::RecordEvent:
	{

	}
        break;
    case NFMonitorType::SceneEvent:
	{

	}
        break;
    default:
        break;
    }
    
}

