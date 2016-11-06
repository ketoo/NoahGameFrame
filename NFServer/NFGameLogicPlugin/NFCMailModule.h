// -------------------------------------------------------------------------
//    @FileName         :   NFCMailModule.h
//    @Author           :   NickYang
//    @Date             :   2012/12/15
//    @Module           :   NFCMailModule
//
// -------------------------------------------------------------------------

#ifndef NFC_MAIL_MODULE_H
#define NFC_MAIL_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIMailModule.h"

class NFCMailModule : public NFIMailModule
{
public:
    NFCMailModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCMailModule() {}

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

public:
    virtual bool SendMail(const NFGUID& reciever, const int nMailType, const NFIDataList& xItemList, const std::string& strNailContent = NULL_STR);
    virtual int OnDrawMail(const NFGUID& self, const NFIDataList& var);
    virtual int OnOpenMail(const NFGUID& self, const NFIDataList& var);
    virtual int OnDeleteMail(const NFGUID& self, const NFIDataList& var);

private:
    NFIKernelModule* m_pKernelModule;
};

#endif // !NFC_MAIL_MODULE_H
