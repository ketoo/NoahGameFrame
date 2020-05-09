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

#include "NFDebugger.h"

void NFDebugger::UpdateOutputData(const NFGUID& runTimeOwner)
{
    if (!runTimeOwner.IsNull())
    {
        auto lvl = GetInputArg(NFDebuggerInputArg::LogLevel);
        auto data = GetInputArg(NFDebuggerInputArg::LogData);

        auto dataFloat = GetInputArg(NFDebuggerInputArg::LogFloat);
        auto dataInt = GetInputArg(NFDebuggerInputArg::LogInt);
        auto dataString = GetInputArg(NFDebuggerInputArg::LogString);
        auto dataObject = GetInputArg(NFDebuggerInputArg::LogObject);
        auto dataVector2 = GetInputArg(NFDebuggerInputArg::LogVector2);
        auto dataVector3 = GetInputArg(NFDebuggerInputArg::LogVector3);
        auto dataDictionary = GetInputArg(NFDebuggerInputArg::LogDictionary);

        std::ostringstream os;

        os << data->GetString();

        os << " float:" << dataFloat->ToString();
        os << " int:" << dataInt->ToString();
        os << " string:" << dataString->ToString();
        os << " object:" << dataObject->ToString();
        os << " vector2:" << dataVector2->ToString();
        os << " vector3:" << dataVector3->ToString();
        os << " dictionary:";

        for (auto it : dataDictionary->dictionaryData)
        {
            os << it.first << ":" << it.second.ToString();
        }

        switch (lvl->GetInt())
        {
            case NFILogModule::NF_LOG_LEVEL::NLL_DEBUG_NORMAL:
                m_pLogModule->LogDebug(runTimeOwner, os);
                break;
            case NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL:
                m_pLogModule->LogInfo(runTimeOwner, os);
                break;
            case NFILogModule::NF_LOG_LEVEL::NLL_WARING_NORMAL:
                m_pLogModule->LogWarning(runTimeOwner, os);
                break;
            case NFILogModule::NF_LOG_LEVEL::NLL_ERROR_NORMAL:
                m_pLogModule->LogError(runTimeOwner, os);
                break;
        default:
            break;
        }
    }
}

NF_SHARE_PTR<NFIOData> NFDebugger::FindOutputNodeIOData()
{
    return GetOutputArg(NFDebuggerOutputArg::NextNode);
}
