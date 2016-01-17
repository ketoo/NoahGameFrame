// -------------------------------------------------------------------------
//    �ļ���      ��    NFIState.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-05 22:11
//    ��������    ��   ״̬��
//
// -------------------------------------------------------------------------

#ifndef _NFI_STATE_H
#define _NFI_STATE_H

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
	MOVE_BY_PLAYER_AI,         // Player's ai
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

    virtual bool DoRule(const NFGUID& self)
    {
        //����true��ʾ���ദ���������ͱ�����
        return false;
    }

    virtual bool Enter(const NFGUID& self)
    {
        return false;
    }

    virtual bool Execute(const NFGUID& self)
    {
        return false;
    }

    virtual bool Exit(const NFGUID& self)
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

    virtual bool DoEnterScript(const NFGUID& self)
    {
        return true;
    }

    virtual bool DoExecuteScript(const NFGUID& self)
    {
        return true;
    }

    virtual bool DoExitScript(const NFGUID& self)
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

};

#endif