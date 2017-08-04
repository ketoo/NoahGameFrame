// -------------------------------------------------------------------------
//    @FileName			:    NFCAIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCAIModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_AI_MODULE_H
#define NFC_AI_MODULE_H

#include "NFIStateMachine.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIAIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHateModule.h"

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

protected:
	int CanUseAnySkill(const NFGUID& self, const NFGUID& other);

	float UseAnySkill(const NFGUID& self, const NFGUID& other);
    
	bool CreateAIObject(const NFGUID& self);

    bool DelAIObject(const NFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    int OnAIObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    //////////////////////////////////////////////////////////////////////////

    void OnBeKilled(const NFGUID& self, const NFGUID& other);

    void OnBeAttack(const NFGUID& self, const NFGUID& other, const int nDamageValue);

    void OnSpring(const NFGUID& self, const NFGUID& other);

    void OnEndSpring(const NFGUID& self, const NFGUID& other);

    void OnMotion(const NFGUID& self, int nResults);

    void OnSelect(const NFGUID& self, const NFGUID& other);

	NFIStateMachine* GetStateMachine(const NFGUID& self);
	NFIState* GetState(const NFAI_STATE eState);

private:
    //状态机
    typedef std::map<NFAI_STATE, NFIState*> TMAPSTATE;
    TMAPSTATE mtStateMap;
    //AI对象，状态机
    typedef std::map<NFGUID, NFIStateMachine*> TOBJECTSTATEMACHINE;
    TOBJECTSTATEMACHINE mtObjectStateMachine;

    NFIHateModule* m_pHateModule;
    NFIKernelModule* m_pKernelModule;
};

#endif