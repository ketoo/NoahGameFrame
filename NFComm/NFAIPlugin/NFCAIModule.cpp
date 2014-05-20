// -------------------------------------------------------------------------
//    �ļ���      ��    NFCAIModule.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-05 22:11
//    ��������    ��    AI��������ƽӿ�
//
// -------------------------------------------------------------------------

#include "NFCAIModule.h"
#include "NFCStateMachine.h"
#include "NFCHateModule.h"
#include "NFAIPlugin.h"

//

NFIKernelModule* NFCAIModule::m_pKernelModule = NULL;
NFIHateModule* NFCAIModule::m_pHateModule = NULL;
NFIEventProcessModule* NFCAIModule::m_pEventProcessModule = NULL;
NFCAIModule* NFCAIModule::m_pThis = NULL;

bool NFCAIModule::Init()
{
    m_pThis = this;
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);

    //m_pEventProcessModule->AddEventCallBack(0, NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, OnDataLoadBeginEvent);
    //m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_WANTO_SWAP_GS, OnSwapGSEvent);

    //////////////////////////////////////////////////////////////////////////

    m_pHateModule = new NFCHateModule(pPluginManager);

    //��ȡ���е�״̬������
    mtStateMap.insert(TMAPSTATE::value_type(DeadState, new NFCDeadState(5.0f, pPluginManager)));
    mtStateMap.insert(TMAPSTATE::value_type(FightState, new NFCFightState(1.0f, pPluginManager)));
    mtStateMap.insert(TMAPSTATE::value_type(IdleState, new NFCIdleState(3.0f, pPluginManager)));
    mtStateMap.insert(TMAPSTATE::value_type(PatrolState, new NFCPatrolState(10.0f, pPluginManager)));


    return true;
}

bool NFCAIModule::Shut()
{
    TOBJECTSTATEMACHINE::iterator itObject = mtObjectStateMachine.begin();
    for (itObject; itObject != mtObjectStateMachine.end(); itObject++)
    {
        delete itObject->second;
    }

    TMAPSTATE::iterator itState = mtStateMap.begin();
    for (itState; itState != mtStateMap.end(); itState++)
    {
        delete itState->second;
    }

    delete m_pHateModule;
    m_pHateModule = NULL;

    return true;
}

NFIState* NFCAIModule::GetState(const int eState)
{
    TMAPSTATE::iterator it = mtStateMap.find((NFAI_STATE)eState);
    if (it != mtStateMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool NFCAIModule::CreateAIObject(const NFIDENTID& self)
{
    //����ֻ��Ϊ���Ժ󷽱�ά��״̬��ʱ�䣬��ԼCPU
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it == mtObjectStateMachine.end())
    {
        mtObjectStateMachine.insert(TOBJECTSTATEMACHINE::value_type(self, new NFCStateMachine(self, this)));

        return true;
    }

    return false;
}

bool NFCAIModule::DelAIObject(const NFIDENTID& self)
{
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        delete it->second;
        mtObjectStateMachine.erase(it);
        return true;
    }

    return false;
}

bool NFCAIModule::Execute(float fFrameTime, float fAllTime)
{
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.begin();
    for (it; it != mtObjectStateMachine.end(); it++)
    {
        it->second->UpData(fFrameTime, fAllTime);
    }

    return true;
}

NFIStateMachine* NFCAIModule::GetStateMachine(const NFIDENTID& self)
{
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        return it->second;
    }

    return NULL;
}

NFIHateModule* NFCAIModule::GetHateModule()
{
    return m_pHateModule;
}

void NFCAIModule::OnBeKilled(const NFIDENTID& self, const NFIDENTID& other)
{
    /*
    ������Ҫչʾһ��״̬ת������Ӧ��
    ��������״̬�⣬��һ��״̬���зǳ���Ĺ���[����˵���е�״̬���ǹ���]
    ����ĳ������[״̬]���ܵ�ĳ���¼���ʱ�򣬽�������һ������[״̬]
    ���������ǿ����õģ���NFIAIRuleConfigModule.h��Ӧ�ÿ��Բ�ѯ�������ת�����
    ��ô�����Ļ������е�NPC�ɿع�����ǲ߻����õĹ���

    Ӧ����˵��״̬�ܹ���Ϊ����״̬
    ��ֹ״̬
    [
    ��������
    ���й���
    ]
    ��Ծ״̬
    [
    Ѳ�߹���
    ս������
    ׷������
    ���Ź���[DoorNpcʹ��]
    ]
    ����״̬
    [
    �Ա�����
    �Ա�����
    ��������
    ]

    ״̬�ڲ����������л�������״̬�����������л�����
    ����������ͬһ��״̬���ܳƣ�����ֻ��һ��ִ����Ϻ��л���������������л���״̬

    �������д�ɵ��ű���ÿ�������Ǽ򵥵��߼�
    Ȼ��NPC�����п���������ӵ�м��ֹ���

    */

    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        it->second->ChangeState(DeadState);
    }
}

void NFCAIModule::OnBeAttack(const NFIDENTID& self, const NFIDENTID& other, const int nDamageValue)
{
    GetHateModule()->AddHate(self, other, nDamageValue);
    GetHateModule()->CompSortList(self);

    NFIStateMachine* pStateMachine = GetStateMachine(self);
    if (pStateMachine)
    {
        if (FightState != pStateMachine->CurrentState())
        {
            pStateMachine->ChangeState(FightState);
        }
    }
}

void NFCAIModule::OnSpring(const NFIDENTID& self, const NFIDENTID& other)
{
    //����ְҵ,�ȼ�,Ѫ��,����
    //ս��״ֻ̬�����б��ڵ��ˣ�Ѳ��,����״̬�������Ҷ����
    NF_AI_SUB_TYPE subType = (NF_AI_SUB_TYPE)m_pKernelModule->QueryPropertyInt(self, "SubType");
    if (NF_AI_SUB_TYPE::NFAST_INITATIVE == subType)
    {
        //��һ���PET����
        const std::string& strClassName = m_pKernelModule->QueryPropertyString(other, "ClassName");
        if ("Player" == strClassName
            || "Pet" == strClassName)
        {
            GetHateModule()->AddHate(self, other, 100);
            GetHateModule()->CompSortList(self);
            NFIStateMachine* pStateMachine = GetStateMachine(self);
            if (pStateMachine)
            {
                if (NFAI_STATE::FightState != pStateMachine->CurrentState())
                {
                    NFIState* pState = GetState(pStateMachine->CurrentState());
                    pState->Execute(self);
                }
            }
        }
    }
}

void NFCAIModule::OnEndSpring(const NFIDENTID& self, const NFIDENTID& other)
{
    GetHateModule()->SetHateValue(self, other, 0);
}

void NFCAIModule::OnMotion(const NFIDENTID& self, int nResults)
{
    //�˶��ɹ�����ʧ�ܣ������������һЩ�������紫�͵�
}

void NFCAIModule::OnSelect(const NFIDENTID& self, const NFIDENTID& other)
{
    //ѡ�������ǽ����񰡣������ǶԻ��������Ǹ���
}

void NFCAIModule::OnReload(const char* pstrModuleName, NFILogicModule* pModule)
{

}

int NFCAIModule::CanUseAnySkill(const NFIDENTID& self, const NFIDENTID& other)
{
    return 0;
}

float NFCAIModule::UseAnySkill(const NFIDENTID& self, const NFIDENTID& other)
{
    return 0.5f;
}

int NFCAIModule::OnAIObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
{
    if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {
        m_pThis->DelAIObject(self);
    }
    else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
    {
        //m_pEventProcessModule->AddEventCallBack(self, NFED_ON_SERVER_MOVEING, OnMoveRquireEvent);
        m_pThis->CreateAIObject(self);
    }

    return 0;
}

bool NFCAIModule::AfterInit()
{
    m_pEventProcessModule->AddClassCallBack("AttackNPC", OnAIObjectEvent);
    m_pEventProcessModule->AddClassCallBack("Pet", OnAIObjectEvent);
    return true;
}

bool NFCAIModule::BeforeShut()
{
    return true;
}
