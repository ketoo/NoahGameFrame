// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldChatModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldChatModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLD_Chat_MODULE_H
#define NFC_WORLD_Chat_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldChatModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"

class NFCWorldChatModule
    : public NFIWorldChatModule
{
public:
    NFCWorldChatModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();


	//////////////////////////////////////////////////////////////////////////
protected:
	void OnChatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

protected:
    NFIKernelModule* m_pKernelModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
private:
};

#endif