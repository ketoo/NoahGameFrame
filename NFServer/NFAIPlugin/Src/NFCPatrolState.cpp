// -------------------------------------------------------------------------
//    �ļ���      ��    NFCPatrolState.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-08 23:26
//    ��������    ��
//
// -------------------------------------------------------------------------

#include "../NFCAIModule.h"

NFCPatrolState::NFCPatrolState(float fHeartBeatTime, NFIPluginManager* p)
    : NFIState(PatrolState, fHeartBeatTime, p)
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pAIModule = pPluginManager->FindModule<NFIAIModule>("NFCAIModule");
    m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>("NFCMoveModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

    m_pHateModule = m_pAIModule->GetHateModule();
}

bool NFCPatrolState::Enter(const NFGUID& self)
{
    if (!NFIState::Enter(self))
    {
        //����Ѳ��״̬
        //�ڳ���������ҵط��߶�Ѳ��
        RandomPatrol(self);
    }

    return true;
}

bool NFCPatrolState::Execute(const NFGUID& self)
{
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
			NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
			NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));

			//����Ƕ���ģ����ߣ�����idle
			switch (eMoveType)
			{
			case NFAI_MOVE_TYPE::MOVE_BY_POINT_LIST:
				{
					//�����Ƿ��п��Թ����Ķ���
					if (!ident.IsNull())
					{
						pStateMachine->ChangeState(FightState);
					}
					else
					{
						//��һ���ڵ�
                        //object��¼��ǰ��·��ID��index���Լ�¼Ѱ·��λ��
					}
				}
				break;

			case NFAI_MOVE_TYPE::MOVE_BY_RANDOM:
				{
					//�����Ƿ��п��Թ����Ķ���
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

bool NFCPatrolState::Exit(const NFGUID& self)
{

    return true;
}

bool NFCPatrolState::DoRule(const NFGUID& self)
{

    return true;
}

bool NFCPatrolState::RandomPatrol(const NFGUID& self)
{
    //���ȣ��ÿ���û·��

    //û�еĻ�������Ҹ���ַ�߰�(�Գ�����Ϊ���Ŀ�ʼ��,���ӵ�ַ)NPCConfigID
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
				//���ܷ�Ѱ·������Ѱ·������
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
				m_pKernelModule->DoEvent(self, NFED_ON_CLIENT_REQUIRE_MOVE, valueList);

				m_pKernelModule->SetPropertyFloat(self, "X", fCurX);
				m_pKernelModule->SetPropertyFloat(self, "Y", fCurY);
				m_pKernelModule->SetPropertyFloat(self, "Z", fCurZ);

				return true;
			}
		}
	}

    return false;
}
