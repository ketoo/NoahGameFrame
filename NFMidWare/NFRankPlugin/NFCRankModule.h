//------------------------------------------------------------------------ -
//    @FileName			:    NFRankPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFRankPlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_RANK_MODULE_H
#define NFC_RANK_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIRankModule.h"

class NFCRankModule : public NFIRankModule
{
public:
    NFCRankModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCRankModule() {}

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

private:
    NFIKernelModule* m_pKernelModule;
};

#endif
