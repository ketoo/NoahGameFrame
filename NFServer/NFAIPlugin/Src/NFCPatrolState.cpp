// -------------------------------------------------------------------------
//    文件名      ：    NFCPatrolState.cpp
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-08 23:26
//    功能描述    ：
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "../NFCAIModule.h"

bool NFCPatrolState::Enter(const NFIDENTID& self)
{
    if (!NFIState::Enter(self))
    {
        //进入巡逻状态
        //在出生点随机找地方走动巡逻
        RandomPatrol(self);
    }

    return true;
}

bool NFCPatrolState::Execute(const NFIDENTID& self)
{
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            //查找是否有可以攻击的对象
            NFIDENTID ident = m_pHateModule->QueryMaxHateObject(self);
            if (!ident.IsNull())
            {
                pStateMachine->ChangeState(FightState);
            }
            else
            {
                RandomPatrol(self);
            }
        }
    }

    return true;
}

bool NFCPatrolState::Exit(const NFIDENTID& self)
{

    return true;
}

bool NFCPatrolState::DoRule(const NFIDENTID& self)
{

    return true;
}

bool NFCPatrolState::RandomPatrol(const NFIDENTID& self)
{
    //首先，得看有没路径

    //没有的话，随机找个地址走吧(以出生点为中心开始找,种子地址)NPCConfigID
    const std::string& strConfigID = m_pKernelModule->GetPropertyString(self, "NPCConfigID");
    if (strConfigID.length() > 0)
    {
        NFIPropertyManager* propertyManager = m_pElementInfoModule->GetPropertyManager(strConfigID.c_str());
        NFIProperty* propertyX =  propertyManager->GetElement("SeedX");
        NFIProperty* propertyY =  propertyManager->GetElement("SeedY");
        NFIProperty* propertyZ =  propertyManager->GetElement("SeedZ");

        float fCurX = propertyX->GetFloat();
        float fCurY = propertyY->GetFloat();
        float fCurZ = propertyZ->GetFloat();

        float fPosOffestX = (float)(rand() / double(RAND_MAX) - 0.5f);
        float fPosOffestZ = (float)(rand() / double(RAND_MAX) - 0.5f);
        fPosOffestX *= 10;
        fPosOffestZ *= 10;

        fCurX += fPosOffestX;
        fCurZ += fPosOffestZ;

        if (fCurX > 0.0f && fCurZ > 0.0f)
        {
            //看能否寻路，不能寻路则重来
            NFObjectStateType eStateType = NFObjectStateType::NOST_RUN;
            float fRand = (float)(rand() / double(RAND_MAX));
            if (fRand < 0.5f)
            {
                eStateType = NFObjectStateType::NOST_WALK;
            }

            NFCDataList valueList;
            valueList.AddFloat(fCurX);
            valueList.AddFloat(fCurY);
            valueList.AddFloat(fCurZ);
            m_pEventProcessModule->DoEvent(self, NFED_ON_CLIENT_REQUIRE_MOVE, valueList);

            return true;
        }

    }

    return false;
}
