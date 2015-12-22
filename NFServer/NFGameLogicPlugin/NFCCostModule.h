// -------------------------------------------------------------------------
//    @FileName         £º    NFCCostModule.h
//    @Author           £º    eliteYang
//    @Date             £º    2015/05/18
//    @Module           £º    NFICostModule
//
// -------------------------------------------------------------------------

#ifndef NFC_COST_MODULE_H
#define NFC_COST_MODULE_H

#include "NFComm/NFPluginModule/NFICostModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCCostModule : public NFICostModule
{
public:
    NFCCostModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCCostModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

    virtual bool Cost(const NFGUID& self, const std::string& strCostID);
    virtual bool Cost(const NFGUID& self, const std::list<std::string>& xCostList);

private:
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementModule;
    NFILogModule* m_pLogModule;
    NFIPropertyModule* m_pPropertyModule;
};

#endif // !__NFC_COST_MODULE_H_