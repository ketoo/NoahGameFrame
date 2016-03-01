// -------------------------------------------------------------------------
//    @FileName         :    NFIAIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIAIModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_AI_MODULE_H
#define _NFI_AI_MODULE_H

#include "NFILogicModule.h"

class NFIState;
class NFIStateMachine;
class NFIKnowLedge;
class NFIWarnModule;
class NFIHateModule;

enum NF_AI_SUB_TYPE
{
    NFAST_TASK,//任务
    NFAST_PASSTIVE,//被动
    NFAST_INITATIVE,//主动
};

class NFIAIModule
    : public NFILogicModule
{
public:
    //得到状态实例
    virtual NFIState* GetState(const int eState) = 0;
    //
    //  //得到状态机
    virtual NFIStateMachine* GetStateMachine(const NFGUID& selfID) = 0;

    //仇恨模块
    virtual NFIHateModule* GetHateModule() = 0;
    virtual NFIKernelModule* GetKernelModule() = 0;
    //////////////////////////////////////////////////////////////////////////
    virtual void OnBeKilled(const NFGUID& self, const NFGUID& other) = 0;

    virtual void OnBeAttack(const NFGUID& self, const NFGUID& other, const int nDamageValue) = 0;

    virtual void OnSpring(const NFGUID& self, const NFGUID& other) = 0;

    virtual void OnEndSpring(const NFGUID& self, const NFGUID& other) = 0;

    //能否对那个人使用技能呢?返回0:可以 1:距离不够 2:CD不到
    virtual int CanUseAnySkill(const NFGUID& self, const NFGUID& other) = 0;

    //返回的是技能施法时间
    virtual float UseAnySkill(const NFGUID& self, const NFGUID& other) = 0;

protected:
private:
};

#endif