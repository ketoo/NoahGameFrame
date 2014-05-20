// -------------------------------------------------------------------------
//    @FileName         :    NFCAIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCAIModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_AI_MODULE_H_
#define _NFC_AI_MODULE_H_

#include "NFIHateModule.h"
#include "NFIStateMachine.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIAIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFCAIModule
    : public NFIAIModule
{
public:
    NFCAIModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();

    virtual bool Shut();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual void OnReload(const char* pstrModuleName, NFILogicModule* pModule);

    virtual NFIState* GetState(const int eState) ;

    virtual NFIStateMachine* GetStateMachine(const NFIDENTID& self);

    virtual NFIHateModule* GetHateModule();

    //找可视范围内的所有人


    int CanUseAnySkill(const NFIDENTID& self, const NFIDENTID& other);

    float UseAnySkill(const NFIDENTID& self, const NFIDENTID& other);
protected:

    virtual bool CreateAIObject(const NFIDENTID& self);

    virtual bool DelAIObject(const NFIDENTID& self);

    //////////////////////////////////////////////////////////////////////////
    static int OnAIObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);


    //////////////////////////////////////////////////////////////////////////

    virtual void OnBeKilled(const NFIDENTID& self, const NFIDENTID& other);

    virtual void OnBeAttack(const NFIDENTID& self, const NFIDENTID& other, const int nDamageValue);

    virtual void OnSpring(const NFIDENTID& self, const NFIDENTID& other);

    virtual void OnEndSpring(const NFIDENTID& self, const NFIDENTID& other);

    virtual void OnMotion(const NFIDENTID& self, int nResults);

    virtual void OnSelect(const NFIDENTID& self, const NFIDENTID& other);


private:
    //状态机
    typedef std::map<NFAI_STATE, NFIState*> TMAPSTATE;
    TMAPSTATE mtStateMap;
    //AI对象，状态机
    typedef std::map<NFIDENTID, NFIStateMachine*> TOBJECTSTATEMACHINE;
    TOBJECTSTATEMACHINE mtObjectStateMachine;

    static NFIHateModule* m_pHateModule;
    static NFIKernelModule* m_pKernelModule;
    static NFIEventProcessModule* m_pEventProcessModule;
    static NFCAIModule* m_pThis;
};

#endif