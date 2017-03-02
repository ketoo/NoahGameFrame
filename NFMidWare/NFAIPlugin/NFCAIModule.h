// -------------------------------------------------------------------------
//    @FileName			:    NFCAIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCAIModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_AI_MODULE_H
#define NFC_AI_MODULE_H

#include "NFIHateModule.h"
#include "NFIStateMachine.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIAIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIModule.h"

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

    virtual bool Execute();

    virtual void OnReload(const char* pstrModuleName, NFIModule* pModule);

    virtual NFIState* GetState(const int eState) ;

    virtual NFIStateMachine* GetStateMachine(const NFGUID& self);

    virtual NFIHateModule* GetHateModule();
    virtual NFIKernelModule* GetKernelModule();
    //找可视范围内的所有人


    int CanUseAnySkill(const NFGUID& self, const NFGUID& other);

    float UseAnySkill(const NFGUID& self, const NFGUID& other);
protected:

    virtual bool CreateAIObject(const NFGUID& self);

    virtual bool DelAIObject(const NFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    virtual int OnAIObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    //////////////////////////////////////////////////////////////////////////

    virtual void OnBeKilled(const NFGUID& self, const NFGUID& other);

    virtual void OnBeAttack(const NFGUID& self, const NFGUID& other, const int nDamageValue);

    virtual void OnSpring(const NFGUID& self, const NFGUID& other);

    virtual void OnEndSpring(const NFGUID& self, const NFGUID& other);

    virtual void OnMotion(const NFGUID& self, int nResults);

    virtual void OnSelect(const NFGUID& self, const NFGUID& other);


private:
    //状态机
    typedef std::map<NFAI_STATE, NFIState*> TMAPSTATE;
    TMAPSTATE mtStateMap;
    //AI对象，状态机
    typedef std::map<NFGUID, NFIStateMachine*> TOBJECTSTATEMACHINE;
    TOBJECTSTATEMACHINE mtObjectStateMachine;

    NFIHateModule* m_pHateModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif