// -------------------------------------------------------------------------
//    文件名      ：    NFCPatrolState.cpp
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-08 23:26
//    功能描述    ：
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "../NFCAIModule.h"

NFCPatrolState::NFCPatrolState(float fHeartBeatTime, NFIPluginManager* p)
    : NFIState(PatrolState, fHeartBeatTime, p)
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pAIModule = dynamic_cast<NFIAIModule*>(pPluginManager->FindModule("NFCAIModule"));
    m_pMoveModule = dynamic_cast<NFIMoveModule*>(pPluginManager->FindModule("NFCMoveModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    m_pHateModule = m_pAIModule->GetHateModule();
}

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
			NFIDENTID ident = m_pHateModule->QueryMaxHateObject(self);
			NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));

			//如果是定点的，则不走，继续idle
			switch (eMoveType)
			{
			case NFAI_MOVE_TYPE::MOVE_BY_POINT_LIST:
				{
					//查找是否有可以攻击的对象
					if (!ident.IsNull())
					{
						pStateMachine->ChangeState(FightState);
					}
					else
					{
						//下一个节点
                        //object记录当前的路径ID和index，以记录寻路的位置
					}
				}
				break;

			case NFAI_MOVE_TYPE::MOVE_BY_RANDOM:
				{
					//查找是否有可以攻击的对象
					if (!ident.IsNull())
					{
						pStateMachine->ChangeState(FightState);
					}
					else
					{
						RandomPatrol(self);
					}
				}
				break;
			default:
				break;
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
    const std::string& strNPCID = m_pKernelModule->GetPropertyString(self, "ConfigID");
    if (!strConfigID.empty())
    {
		NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigID);
		if (xPropertyManager)
		{
			NF_SHARE_PTR<NFIProperty> xPropertyX =  xPropertyManager->GetElement("SeedX");
			NF_SHARE_PTR<NFIProperty> xPropertyY =  xPropertyManager->GetElement("SeedY");
			NF_SHARE_PTR<NFIProperty> xPropertyZ =  xPropertyManager->GetElement("SeedZ");

			float fCurX = xPropertyX->GetFloat();
			float fCurY = xPropertyY->GetFloat();
			float fCurZ = xPropertyZ->GetFloat();

			float fPosOffestX = (float)(rand() / double(RAND_MAX) - 0.5f);
			float fPosOffestZ = (float)(rand() / double(RAND_MAX) - 0.5f);
			fPosOffestX *= 10;
			fPosOffestZ *= 10;

			fCurX += fPosOffestX;
			fCurZ += fPosOffestZ;

			//if (fCurX > 0.0f && fCurZ > 0.0f)
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

				m_pKernelModule->SetPropertyFloat(self, "X", fCurX);
				m_pKernelModule->SetPropertyFloat(self, "Y", fCurY);
				m_pKernelModule->SetPropertyFloat(self, "Z", fCurZ);

				return true;
			}
		}
	}

    return false;
}
