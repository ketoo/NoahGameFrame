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

#include "NFExecuter.h"

void NFCreateObjectExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFCreateObjectExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
    if (!iteration)
    {
        //create object
    }
}

NF_SHARE_PTR<NFIOData> NFCreateObjectExecuter::FindOutputNodeIOData()
{
    return GetOutputArg(NFExecuterCreateObjectOutputputArg::NextNode);
}

void NFDestroyObjectExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFDestroyObjectExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFDestroyObjectExecuter::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFGameEventExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFGameEventExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFGameEventExecuter::FindOutputNodeIOData()
{
    return NF_SHARE_PTR<NFIOData>();
}

void NFMoveObjectExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFMoveObjectExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFMoveObjectExecuter::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFEnterSceneExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFEnterSceneExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFEnterSceneExecuter::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFAddHeartBeatExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFAddHeartBeatExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFAddHeartBeatExecuter::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFAttackObjectExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFAttackObjectExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFAttackObjectExecuter::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFUseSkillExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFUseSkillExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFUseSkillExecuter::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFUseItemExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFUseItemExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFUseItemExecuter::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFSleepExecuter::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

void NFSleepExecuter::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFSleepExecuter::FindOutputNodeIOData()
{
    return NF_SHARE_PTR<NFIOData>();
}
