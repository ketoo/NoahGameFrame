// -------------------------------------------------------------------------
//    @FileName			:    NFCPatrolState.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCPatrolState
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFIStateMachine.h"
#include "NFCPatrolState.h"

NFCPatrolState::NFCPatrolState(float fHeartBeatTime, NFIPluginManager* p)
	: NFIState(PatrolState, fHeartBeatTime, p)
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pAIModule = pPluginManager->FindModule<NFIAIModule>();
	m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
}

bool NFCPatrolState::Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Enter(self, pStateMachine))
    {
        RandomPatrol(self, pStateMachine);
    }

    return true;
}

bool NFCPatrolState::Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Execute(self, pStateMachine))
    {
			NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
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
						RandomPatrol(self, pStateMachine);
					}
				}
				break;
			default:
				break;
			}
    }

    return true;
}

bool NFCPatrolState::Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
{

    return true;
}

bool NFCPatrolState::DoRule(const NFGUID& self, NFIStateMachine* pStateMachine)
{

    return true;
}

bool NFCPatrolState::RandomPatrol(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    //首先，得看有没路径

    //没有的话，随机找个地址走吧(以出生点为中心开始找,种子地址)NPCConfigID
    const std::string& strConfigID = m_pKernelModule->GetPropertyString(self, "NPCConfigID");
    const std::string& strNPCID = m_pKernelModule->GetPropertyString(self, "ConfigID");
    if (!strConfigID.empty())
    {
		NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pElementModule->GetPropertyManager(strConfigID);
		if (xPropertyManager)
		{
			NF_SHARE_PTR<NFIProperty> xPropertyX =  xPropertyManager->GetElement("SeedX");
			NF_SHARE_PTR<NFIProperty> xPropertyY =  xPropertyManager->GetElement("SeedY");
			NF_SHARE_PTR<NFIProperty> xPropertyZ =  xPropertyManager->GetElement("SeedZ");

			double fCurX = xPropertyX->GetFloat();
			double fCurY = xPropertyY->GetFloat();
			double fCurZ = xPropertyZ->GetFloat();

			double fPosOffestX = (double)(rand() / double(RAND_MAX) - 0.5);
			double fPosOffestZ = (double)(rand() / double(RAND_MAX) - 0.5);
			fPosOffestX *= 10;
			fPosOffestZ *= 10;

			fCurX += fPosOffestX;
			fCurZ += fPosOffestZ;

			//if (fCurX > 0.0f && fCurZ > 0.0f)
			{
				//看能否寻路，不能寻路则重来
				NFAI_STATE eStateType = NFAI_STATE::ChaseState;
				double fRand = (double)(rand() / double(RAND_MAX));
				if (fRand < 0.5f)
				{
					eStateType = NFAI_STATE::ChaseState;
				}

				NFDataList valueList;
				valueList.AddFloat(fCurX);
				valueList.AddFloat(fCurY);
				valueList.AddFloat(fCurZ);
				//m_pKernelModule->DoEvent(self, NFED_ON_CLIENT_REQUIRE_MOVE, valueList);

				m_pKernelModule->SetPropertyFloat(self, "X", fCurX);
				m_pKernelModule->SetPropertyFloat(self, "Y", fCurY);
				m_pKernelModule->SetPropertyFloat(self, "Z", fCurZ);

				return true;
			}
		}
	}

    return false;
}
