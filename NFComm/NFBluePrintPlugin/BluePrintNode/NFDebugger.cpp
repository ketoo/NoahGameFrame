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

void NFDebugger::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFDebugger::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
    if (!runTimeOnwer.IsNull())
    {
        auto lvl = GetInputArg(NFDebuggerInputArg::LogLevel);
        auto data = GetInputArg(NFDebuggerInputArg::LogData);

        std::ostringstream os;

        os << data->varData.GetString();

        switch (lvl->varData.GetInt())
        {
            case NFILogModule::NF_LOG_LEVEL::NLL_DEBUG_NORMAL:
                m_pLogModule->LogDebug(runTimeOnwer, os);
                break;
            case NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL:
                m_pLogModule->LogInfo(runTimeOnwer, os);
                break;
            case NFILogModule::NF_LOG_LEVEL::NLL_WARING_NORMAL:
                m_pLogModule->LogWarning(runTimeOnwer, os);
                break;
            case NFILogModule::NF_LOG_LEVEL::NLL_ERROR_NORMAL:
                m_pLogModule->LogError(runTimeOnwer, os);
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
