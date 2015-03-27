// -------------------------------------------------------------------------
//    �ļ���      ��    NFIState.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-05 22:11
//    ��������    ��   ״̬��
//
// -------------------------------------------------------------------------

#ifndef _NFI_STATE_H_
#define _NFI_STATE_H_

#include "NFComm/NFCore/NFIDataList.h"

class NFIMoveModule;
class NFIAIModule;
class NFIHateModule;
class NFIKernelModule;
class NFIEventProcessModule;
class NFIElementInfoModule;
class NFIPluginManager;

//���е�״̬
enum NFAI_MOVE_TYPE
{
	NO_MOVE_TYPE,              // ���ߵģ�����
	MOVE_BY_POINT_LIST,        // ·��Ѳ��
	MOVE_BY_RANDOM,            // ���Ѳ��
};

//���е�״̬
enum NFAI_STATE
{
    State_Error = 0,
    IdleState,                  // ����״̬
    PatrolState,                // Ѳ��״̬
    FightState,                 // ս��״̬
    ChaseState,                 // ׷��״̬
    DeadState,                  // ����״̬
};

class NFIState
{
public:
    NFIState(NFAI_STATE eState, const float fHeartBeatTime, NFIPluginManager* p)
    {
        meState = eState;
        mfHeartBeatTime = fHeartBeatTime;
        pPluginManager = p;
    }

    virtual ~NFIState()
    {
    }

    virtual bool DoRule(const NFIDENTID& self)
    {
        //����true��ʾ���ദ���������ͱ�����
        return false;
    }

    virtual bool Enter(const NFIDENTID& self)
    {
        return false;
    }

    virtual bool Execute(const NFIDENTID& self)
    {
        return false;
    }

    virtual bool Exit(const NFIDENTID& self)
    {
        return false;
    }

    const NFAI_STATE GetCurrState()
    {
        return meState;
    }

    float GetHeartBeatTime()
    {
        return mfHeartBeatTime;
    }

protected:

    virtual bool DoEnterScript(const NFIDENTID& self)
    {
        return true;
    }

    virtual bool DoExecuteScript(const NFIDENTID& self)
    {
        return true;
    }

    virtual bool DoExitScript(const NFIDENTID& self)
    {
        return true;
    }

protected:
    NFAI_STATE  meState;
    float               mfHeartBeatTime;
    NFIPluginManager* pPluginManager;

    NFIAIModule* m_pAIModule;
    NFIHateModule* m_pHateModule;
    NFIKernelModule* m_pKernelModule;
    NFIMoveModule* m_pMoveModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIEventProcessModule* m_pEventProcessModule;

};

#endif