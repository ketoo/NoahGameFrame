// -------------------------------------------------------------------------
//    @FileName			:    NFCMailModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCMailModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_MAIL_MODULE_H
#define NFC_MAIL_MODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIMailModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
////////////////////////////////////////////////////////////////////////////



class NFCMailModule
    : public NFIMailModule
{
public:
	explicit NFCMailModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

	virtual bool Init() override;
	virtual bool Shut() override;
	virtual bool Execute() override;

	virtual bool AfterInit() override;

public:
	virtual bool SendMail(const NFGUID& reciever, const int nMailType, const NFDataList& xItemList, const std::string& strNailContent = NULL_STR) { return false; }
	virtual int OnDrawMail(const NFGUID& self, const NFDataList& var) { return 0; }
	virtual int OnOpenMail(const NFGUID& self, const NFDataList& var) { return 0; }
	virtual int OnDeleteMail(const NFGUID& self, const NFDataList& var) { return 0; }

protected:

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIEventModule* m_pEventModule;
	NFISceneAOIModule* m_pSceneAOIModule;
    //////////////////////////////////////////////////////////////////////////
};
#endif
