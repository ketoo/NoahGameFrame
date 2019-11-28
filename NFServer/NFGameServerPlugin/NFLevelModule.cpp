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


#include "NFLevelModule.h"

bool NFLevelModule::Init()
{
    return true;
}


bool NFLevelModule::Shut()
{
    return true;
}

bool NFLevelModule::Execute()
{
    
    return true;
}

bool NFLevelModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	
    return true;
}

int NFLevelModule::AddExp(const NFGUID& self, const int64_t nExp)
{
    int eJobType = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Job());
    int64_t nCurExp = m_pKernelModule->GetPropertyInt(self, NFrame::Player::EXP());
    int nLevel = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Level());
    int64_t nMaxExp = (int)m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());

    nCurExp += nExp;

    int64_t nRemainExp = nCurExp - nMaxExp;
    while (nRemainExp >= 0)
    {
        
        nLevel++;
        
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Level(), nLevel);

        nCurExp = nRemainExp;

        nMaxExp = (int)m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());
        if (nMaxExp <= 0)
        {
            break;
        }

        nRemainExp -= nMaxExp;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::EXP(), nCurExp);

    return 0;
}