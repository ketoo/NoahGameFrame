// -------------------------------------------------------------------------
//    �ļ���      ��    NFCPatrolState.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-08 23:26
//    ��������    ��
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "../NFCAIModule.h"

bool NFCPatrolState::Enter(const NFIDENTID& self)
{
    if (!NFIState::Enter(self))
    {
        //����Ѳ��״̬
        //�ڳ���������ҵط��߶�Ѳ��
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
            //�����Ƿ��п��Թ����Ķ���
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
    //���ȣ��ÿ���û·��

    //û�еĻ�������Ҹ���ַ�߰�(�Գ�����Ϊ���Ŀ�ʼ��,���ӵ�ַ)NPCConfigID
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
            m_pEventProcessModule->DoEvent(self, NFED_ON_CLIENT_REQUIRE_MOVE, valueList);

            return true;
        }

    }

    return false;
}
