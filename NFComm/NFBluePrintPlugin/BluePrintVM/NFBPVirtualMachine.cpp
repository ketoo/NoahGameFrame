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

int NFBPVirtualMachine::GameEventIDCallBack(const int eventID, const NFDataList& dataList)
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
        if (inputEventID->varData.empty())
        {
            m_pLogModule->LogError(monitor->name + " The monitor's Input EventID is empty.");
            return;
        }

        try
        {
            int eventID = std::stoi(inputEventID->varData);
            m_pEventModule->AddEventCallBack(eventID, this, &NFBPVirtualMachine::GameEventIDCallBack);
        }
        catch (const std::exception&)
        {
            m_pLogModule->LogError(monitor->name + " The monitor's Input EventID must as a integer: " + inputEventID->varData);
            return;
        }
       
    }
        break;
    case NFMonitorType::NetworkEvent:
        break;
    case NFMonitorType::NetworkMsgEvent:
        break;
    case NFMonitorType::ObjectEvent:
        break;
    case NFMonitorType::PropertyEvent:
        break;
    case NFMonitorType::RecordEvent:
        break;
    case NFMonitorType::SceneEvent:
        break;
    case NFMonitorType::ItemEvent:
        break;
    case NFMonitorType::SkillEvent:
        break;
    case NFMonitorType::BuffEvent:
        break;
    default:
        break;
    }
    
}
