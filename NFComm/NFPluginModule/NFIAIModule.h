// -------------------------------------------------------------------------
//    @FileName         :    NFIAIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIAIModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_AI_MODULE_H_
#define _NFI_AI_MODULE_H_

#include "NFILogicModule.h"

class NFIState;
class NFIStateMachine;
class NFIKnowLedge;
class NFIWarnModule;
class NFIHateModule;

enum NF_AI_SUB_TYPE 
{
	NFAST_TASK,//����
	NFAST_PASSTIVE,//����
	NFAST_INITATIVE,//����	
};

class NFIAIModule
    : public NFILogicModule
{
public:
    //�õ�״̬ʵ��
    virtual NFIState* GetState(const int eState) = 0;
    //
    //  //�õ�״̬��
    virtual NFIStateMachine* GetStateMachine(const NFIDENTID& selfID) = 0;

    //���ģ��
    virtual NFIHateModule* GetHateModule() = 0;
    virtual NFIKernelModule* GetKernelModule() = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void OnBeKilled(const NFIDENTID& self, const NFIDENTID& other) = 0;

	virtual void OnBeAttack(const NFIDENTID& self, const NFIDENTID& other, const int nDamageValue) = 0;

	virtual void OnSpring(const NFIDENTID& self, const NFIDENTID& other) = 0;

	virtual void OnEndSpring(const NFIDENTID& self, const NFIDENTID& other) = 0;

	//�ܷ���Ǹ���ʹ�ü�����?����0:���� 1:���벻�� 2:CD����
	virtual int CanUseAnySkill(const NFIDENTID& self, const NFIDENTID& other) = 0;
	
	//���ص��Ǽ���ʩ��ʱ��
	virtual float UseAnySkill(const NFIDENTID& self, const NFIDENTID& other) = 0;

protected:
private:
};

#endif